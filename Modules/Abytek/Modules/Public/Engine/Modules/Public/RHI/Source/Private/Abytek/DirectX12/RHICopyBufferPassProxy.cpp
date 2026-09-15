#include "Abytek/DirectX12/RHICopyBufferPassProxy.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHICopyBufferPassProxy::Build(const TW_Valid<A_RHICopyBufferPass>& Pass)
    {
        A_RHICopyBufferPassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
    }   
    void F_DirectX12RHICopyBufferPassProxy::Release()
    {
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHICopyBufferPassProxy::Release();
    }

    void F_DirectX12RHICopyBufferPassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        ExecuteParams.D3D12CommandList->CopyBufferRegion(
            GetDstBufferProxy().FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource().Get(),
            GetDstOffsetInBytes(),
            GetSrcBufferProxy().FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource().Get(),
            GetSrcOffsetInBytes(),
            GetSizeInBytes()
        );
    }
}
#endif