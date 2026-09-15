#include "Abytek/DirectX12/RHIClearRTVPassProxy.hpp"

#include "Abytek/DirectX12/RHIResourceViewProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIClearRTVPassProxy::Build(const TW_Valid<A_RHIClearRTVPass>& Pass)
    {
        A_RHIClearRTVPassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
    }   
    void F_DirectX12RHIClearRTVPassProxy::Release()
    {
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIClearRTVPassProxy::Release();
    }

    void F_DirectX12RHIClearRTVPassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        auto CastedRTV = GetRTVProxy().FastCast<F_DirectX12RHIResourceViewProxy>();
        
        auto Color = GetColor();
        float ColorF32x4[4] = { Color.X, Color.Y, Color.Z, Color.W };
        
        D3D12_CPU_DESCRIPTOR_HANDLE RTVCPUDescriptorHandle = CastedRTV->GetDescriptorRange().GetCPUHandle(false);
        
        ExecuteParams.D3D12CommandList->ClearRenderTargetView(
              RTVCPUDescriptorHandle,
              ColorF32x4,
              0,
              nullptr
        );
    }
}
#endif