#include "Abytek/DirectX12/RHIReadbackBufferPass.hpp"
#include "Abytek/DirectX12/RHICopyBufferPass.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIReadbackBufferPass::Build(const F_RHIReadbackBufferPassBuildParams& BuildParams)
    {
        A_RHIReadbackBufferPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
        
        auto Context = GetContext();
        
        if (!FlagHas(BuildParams.Buffer->GetAccessCapabilities().CPU, E_RHIResourceCPUAccess::WRITE))
        {
            _TransientReadbackBufferRange = Context->GetTransientReadbackBufferManager()->Allocate(
                GetSizeInBytes()
            );
            
            F_RHICopyBufferPassBuildParams CopyBufferPassBuildParams;
            CopyBufferPassBuildParams.Context = Context;
            CopyBufferPassBuildParams.DstBuffer = _TransientReadbackBufferRange.GetBuffer();
            CopyBufferPassBuildParams.DstOffsetInBytes = _TransientReadbackBufferRange.BeginOffsetInBytes;
            CopyBufferPassBuildParams.SrcBuffer = BuildParams.Buffer;
            CopyBufferPassBuildParams.SrcOffsetInBytes = BuildParams.OffsetInBytes;
            CopyBufferPassBuildParams.SizeInBytes = GetSizeInBytes();
            _CopyPass = RACreateAndBuildShared<A_RHICopyBufferPass>(CopyBufferPassBuildParams);
        }
    }   
    void F_DirectX12RHIReadbackBufferPass::Release()
    {
        _CopyPass = {};
        _TransientReadbackBufferRange = {};
        
        A_DirectX12RHIPassExtension::Release();
        A_RHIReadbackBufferPass::Release();
    }

    void F_DirectX12RHIReadbackBufferPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        
        if (_TransientReadbackBufferRange)
        {
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        _TransientReadbackBufferRange.GetBuffer().Weak(),
                        0
                    ),
                    F_RHIResourceAccess::MakeReadback()
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
                    F_RHIResourceAccess::MakeReadback()
                )
            );
        }
    }

    E_DirectX12RHIPassBatchType F_DirectX12RHIReadbackBufferPass::GetPassBatchType()
    {
        return E_DirectX12RHIPassBatchType::CPU_SYNC;
    }

    void F_DirectX12RHIReadbackBufferPass::OnAddItemsBefore(I_RHISubmissionItemContainer& Container)
    {
        if (_CopyPass)
        {
            Container.AddSubmissionItem(_CopyPass);
        }
    }

    B8 F_DirectX12RHIReadbackBufferPass::CanDetachCopyPass()
    {
        return static_cast<B8>(_CopyPass);
    }
    void F_DirectX12RHIReadbackBufferPass::DetachCopyPass(I_RHISubmissionItemContainer& SubmissionItemContainer)
    {
        ABYTEK_ENGINE_RHI_ASSERT(_CopyPass) << "Cannot detach copy pass";
        SubmissionItemContainer.AddSubmissionItem(_CopyPass);
        _CopyPass = {};
    }

#ifdef ABYTEK_DEBUG_INFO
    void F_DirectX12RHIReadbackBufferPass::SetDebugName(const F_DebugName& Value) noexcept
    {
        A_RHIReadbackBufferPass::SetDebugName(Value);
        if (_CopyPass)
        {
            _CopyPass->SetDebugName(F_Name(ToText(*GetDebugName()) + ABYTEK_TEXT(".CopyPass")));
        }
    }
#endif
}
#endif