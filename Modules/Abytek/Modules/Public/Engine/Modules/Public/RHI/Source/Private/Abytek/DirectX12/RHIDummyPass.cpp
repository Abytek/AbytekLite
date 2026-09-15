#include "Abytek/DirectX12/RHIDummyPass.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIBindGroup.hpp"
#include "Abytek/DirectX12/RHIResourceView.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIDummyPass::Build(const F_RHIDummyPassBuildParams& BuildParams)
    {
        A_RHIDummyPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
    }   
    void F_DirectX12RHIDummyPass::Release()
    {
        A_DirectX12RHIPassExtension::Release();
        A_RHIDummyPass::Release();
    }

    void F_DirectX12RHIDummyPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        
        for (const auto& ResourceView : GetResourceViews())
        {
            ResourceView.FastCast<F_DirectX12RHIResourceView>()->AppendSubresourceBindingSet(SubresourceBindingSet);
        }
        for (const auto& VertexBufferBinding : GetVertexBufferBindings())
        {
            H_DirectX12RHIVertexBufferBinding::AppendSubresourceBindingSet(
                VertexBufferBinding,
                SubresourceBindingSet
            );
        }
        if (const auto& IndexBufferBinding = GetIndexBufferBinding())
        {
            H_DirectX12RHIIndexBufferBinding::AppendSubresourceBindingSet(
                *IndexBufferBinding,
                SubresourceBindingSet
            );
        }
    }
}
#endif 
 