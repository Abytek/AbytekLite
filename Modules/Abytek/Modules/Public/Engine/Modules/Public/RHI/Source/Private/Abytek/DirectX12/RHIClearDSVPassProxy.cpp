#include "Abytek/DirectX12/RHIClearDSVPassProxy.hpp"
#include "Abytek/DirectX12/RHIResourceViewProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIClearDSVPassProxy::Build(const TW_Valid<A_RHIClearDSVPass>& Pass)
    {
        A_RHIClearDSVPassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
    }   
    void F_DirectX12RHIClearDSVPassProxy::Release()
    {
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIClearDSVPassProxy::Release();
    }

    void F_DirectX12RHIClearDSVPassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        auto CastedDSV = GetDSVProxy().FastCast<F_DirectX12RHIResourceViewProxy>();
        
        D3D12_CPU_DESCRIPTOR_HANDLE DSVCPUDescriptorHandle = CastedDSV->GetDescriptorRange().GetCPUHandle(false);
        
        D3D12_CLEAR_FLAGS ClearFlags = D3D12_CLEAR_FLAGS(0);
        
        auto Flags = GetFlags();
        if (FlagHas(Flags, E_RHIClearDSVFlag::DEPTH))
        {
            ClearFlags |= D3D12_CLEAR_FLAG_DEPTH;
        }
        if (FlagHas(Flags, E_RHIClearDSVFlag::STENCIL))
        {
            ClearFlags |= D3D12_CLEAR_FLAG_STENCIL;
        }
        ABYTEK_ENGINE_RHI_ASSERT(ClearFlags) << "Invalid clear flags";
        
        ExecuteParams.D3D12CommandList->ClearDepthStencilView(
              DSVCPUDescriptorHandle,
              ClearFlags,
              GetDepth(),
              GetStencil(),
              0,
              nullptr
        );
    }
}
#endif