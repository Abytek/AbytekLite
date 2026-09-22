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
        if (HasWork())
        {
            _TransientUploadBufferRange = Context->GetTransientUploadBufferManager_V2()->Allocate(
                BuildParams.BufferDataView.size()    
            );
            _TransientUploadBufferRange.Upload(BuildParams.BufferDataView);
        }
    }   
    void F_DirectX12RHIUploadBufferPass::Release()
    {
        _TransientUploadBufferRange = {};
        
        A_DirectX12RHIPassExtension::Release();
        A_RHIUploadBufferPass::Release();
    }

    void F_DirectX12RHIUploadBufferPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        if (HasWork())
        {
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        _TransientUploadBufferRange.GetBuffer().Weak(),
                        0
                    ),
                    F_RHIResourceAccess::MakeCopySrc()
                )
            );
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        GetBuffer().Weak(),
                        0
                    ),
                    F_RHIResourceAccess::MakeCopyDest()
                )
            );
        }
    }
}
#endif