#include "Abytek/DirectX12/RHIUploadBufferPass.hpp"
#include "Abytek/RHICopyBufferPass.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"
#include "Abytek/DirectX12/RHICopyBufferPass.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIUploadBufferPass::Build(const F_RHIUploadBufferPassBuildParams& BuildParams)
    {
        A_RHIUploadBufferPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
        
        auto Context = GetContext();
        
        if (!FlagHas(BuildParams.Buffer->GetAccessCapabilities().CPU, E_RHIResourceCPUAccess::WRITE))
        {
            _TransientUploadBufferRange = Context->GetTransientUploadBufferManager()->Allocate(
                BuildParams.BufferDataView.size()    
            );
            
            F_RHICopyBufferPassBuildParams CopyBufferPassBuildParams;
            CopyBufferPassBuildParams.Context = Context;
            CopyBufferPassBuildParams.DstBuffer = BuildParams.Buffer;
            CopyBufferPassBuildParams.DstOffsetInBytes = BuildParams.OffsetInBytes;
            CopyBufferPassBuildParams.SrcBuffer = _TransientUploadBufferRange.GetBuffer();
            CopyBufferPassBuildParams.SrcOffsetInBytes = _TransientUploadBufferRange.BeginOffsetInBytes;
            CopyBufferPassBuildParams.SizeInBytes = BuildParams.BufferDataView.size();
            _CopyPass = RACreateAndBuildShared<A_RHICopyBufferPass>(CopyBufferPassBuildParams);
        }
    }   
    void F_DirectX12RHIUploadBufferPass::Release()
    {
        _CopyPass = {};
        _TransientUploadBufferRange = {};
        
        A_DirectX12RHIPassExtension::Release();
        A_RHIUploadBufferPass::Release();
    }

    void F_DirectX12RHIUploadBufferPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        
        if (_TransientUploadBufferRange)
        {
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        _TransientUploadBufferRange.GetBuffer().Weak(),
                        0
                    ),
                    F_RHIResourceAccess::MakeUpload()
                )
            );
        }
        else
        {
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        GetBuffer().Weak(),
                        0
                    ),
                    F_RHIResourceAccess::MakeUpload()
                )
            );
        }
    }

    E_DirectX12RHIPassBatchType F_DirectX12RHIUploadBufferPass::GetPassBatchType()
    {
        return E_DirectX12RHIPassBatchType::CPU_SYNC;
    }

    void F_DirectX12RHIUploadBufferPass::OnAddItemsAfter(I_RHISubmissionItemContainer& Container)
    {
        if (_CopyPass)
        {
            Container.AddSubmissionItem(_CopyPass);
        }
    }

    B8 F_DirectX12RHIUploadBufferPass::CanDetachCopyPass()
    {
        return static_cast<B8>(_CopyPass);
    }
    void F_DirectX12RHIUploadBufferPass::DetachCopyPass(I_RHISubmissionItemContainer& SubmissionItemContainer)
    {
        ABYTEK_ENGINE_RHI_ASSERT(_CopyPass) << "Cannot detach copy pass";
        SubmissionItemContainer.AddSubmissionItem(_CopyPass);
        _CopyPass = {};
    }

#ifdef ABYTEK_DEBUG_INFO
    void F_DirectX12RHIUploadBufferPass::SetDebugName(const F_DebugName& Value) noexcept
    {
        A_RHIUploadBufferPass::SetDebugName(Value);
        if (_CopyPass)
        {
            _CopyPass->SetDebugName(F_Name(ToText(*GetDebugName()) + ABYTEK_TEXT(".CopyPass")));
        }
    }
#endif
}
#endif