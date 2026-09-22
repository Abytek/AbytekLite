#include "Abytek/DirectX12/RHIUploadBufferPassProxy.hpp"
#include "Abytek/DirectX12/RHIUploadBufferPass.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIUploadBufferPassProxy::Build(const TW_Valid<A_RHIUploadBufferPass>& Pass)
    {
        A_RHIUploadBufferPassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
        
        auto CastedPass = Pass.FastCast<F_DirectX12RHIUploadBufferPass>();
        if (HasWork())
        {
            const auto& TransientUploadBufferRange = CastedPass->GetTransientUploadBufferRange();
            _UploadBufferProxy = TransientUploadBufferRange.GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
            _UploadBufferOffsetInBytes = TransientUploadBufferRange.BeginOffsetInBytes;
        }
    }   
    void F_DirectX12RHIUploadBufferPassProxy::Release()
    {
        _UploadBufferOffsetInBytes = 0;
        _UploadBufferProxy = {};
        
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIUploadBufferPassProxy::Release();
    }

    void F_DirectX12RHIUploadBufferPassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        if (HasWork())
        {
            auto DstD3D12Resource = GetBufferProxy().FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
            auto DstOffsetInBytes = GetOffsetInBytes();
            auto SrcD3D12Resource = _UploadBufferProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
            auto SrcOffsetInBytes = _UploadBufferOffsetInBytes;
        
            ExecuteParams.D3D12CommandList->CopyBufferRegion(
                DstD3D12Resource.Get(),
                DstOffsetInBytes,
                SrcD3D12Resource.Get(),
                SrcOffsetInBytes,
                GetBufferDataView().size()
            );
        }
    }
}
#endif