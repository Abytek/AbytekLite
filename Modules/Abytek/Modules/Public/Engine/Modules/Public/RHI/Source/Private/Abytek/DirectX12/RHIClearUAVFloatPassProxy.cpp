#include "Abytek/DirectX12/RHIClearUAVFloatPassProxy.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIResourceViewProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIClearUAVFloatPassProxy::Build(const TW_Valid<A_RHIClearUAVFloatPass>& Pass)
    {
        A_RHIClearUAVFloatPassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
    }   
    void F_DirectX12RHIClearUAVFloatPassProxy::Release()
    {
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIClearUAVFloatPassProxy::Release();
    }

    void F_DirectX12RHIClearUAVFloatPassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        auto CastedUAV = GetUAVProxy().FastCast<F_DirectX12RHIResourceViewProxy>();
        
        auto Value = GetValue();
        float ValueF32x4[4] = { Value.X, Value.Y, Value.Z, Value.W };
        
        D3D12_GPU_DESCRIPTOR_HANDLE UAVGPUDescriptorHandle = CastedUAV->GetDescriptorRange().GetGPUHandle();
        D3D12_CPU_DESCRIPTOR_HANDLE UAVCPUDescriptorHandle = CastedUAV->GetDescriptorRange().GetCPUHandle(false);
        
        ExecuteParams.D3D12CommandList->ClearUnorderedAccessViewFloat(
              UAVGPUDescriptorHandle,
              UAVCPUDescriptorHandle,
              CastedUAV->GetResourceProxy().FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource().Get(),
              ValueF32x4,
              0,
              nullptr
        );
    }
}
#endif