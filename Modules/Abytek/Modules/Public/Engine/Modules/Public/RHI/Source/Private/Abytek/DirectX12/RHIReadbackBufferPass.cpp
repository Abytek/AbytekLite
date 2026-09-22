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
        if (HasWork())
        {
            _TransientReadbackBufferRange = Context->GetTransientReadbackBufferManager_V2()->Allocate(
                GetSizeInBytes()
            );
            _TransientReadbackBufferRange.Readback(
                MoveCallback(),
                GetSizeInBytes()
            );
        }
    }   
    void F_DirectX12RHIReadbackBufferPass::Release()
    {
        _TransientReadbackBufferRange = {};
        
        A_DirectX12RHIPassExtension::Release();
        A_RHIReadbackBufferPass::Release();
    }

    void F_DirectX12RHIReadbackBufferPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        if (HasWork())
        {
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        _TransientReadbackBufferRange.GetBuffer().Weak(),
                        0
                    ),
                    F_RHIResourceAccess::MakeCopyDest()
                )
            );
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        GetBuffer().Weak(),
                        0
                    ),
                    F_RHIResourceAccess::MakeCopySrc()
                )
            );
        }
    }

    E_DirectX12RHIPassBatchType F_DirectX12RHIReadbackBufferPass::GetPassBatchType()
    {
        return E_DirectX12RHIPassBatchType::GPU;
    }
}
#endif