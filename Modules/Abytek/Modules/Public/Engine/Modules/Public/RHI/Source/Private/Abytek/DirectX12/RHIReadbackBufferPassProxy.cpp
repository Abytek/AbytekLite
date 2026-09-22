#include "Abytek/DirectX12/RHIReadbackBufferPassProxy.hpp"
#include "Abytek/DirectX12/RHIReadbackBufferPass.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIReadbackBufferPassProxy::Build(const TW_Valid<A_RHIReadbackBufferPass>& Pass)
    {
        A_RHIReadbackBufferPassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
        
        auto CastedPass = Pass.FastCast<F_DirectX12RHIReadbackBufferPass>();
        if (HasWork())
        {
            const auto& TransientReadbackBufferRange = CastedPass->GetTransientReadbackBufferRange();
            _ReadbackBufferProxy = TransientReadbackBufferRange.GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
            _ReadbackBufferOffsetInBytes = TransientReadbackBufferRange.BeginOffsetInBytes;
        }
    }   
    void F_DirectX12RHIReadbackBufferPassProxy::Release()
    {
        _ReadbackBufferOffsetInBytes = 0;
        _ReadbackBufferProxy = {};
        
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIReadbackBufferPassProxy::Release();
    }

    void F_DirectX12RHIReadbackBufferPassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        if (HasWork())
        {
            auto SrcD3D12Resource = GetBufferProxy().FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
            auto SrcOffsetInBytes = GetOffsetInBytes();
            auto DstD3D12Resource = _ReadbackBufferProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
            auto DstOffsetInBytes = _ReadbackBufferOffsetInBytes;
        
            ExecuteParams.D3D12CommandList->CopyBufferRegion(
                DstD3D12Resource.Get(),
                DstOffsetInBytes,
                SrcD3D12Resource.Get(),
                SrcOffsetInBytes,
                GetSizeInBytes()
            );
        }
    }
}
#endif