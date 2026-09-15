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
        const auto& TransientUploadBufferRange = CastedPass->GetTransientUploadBufferRange();
        if (TransientUploadBufferRange)
        {
            _UploadBufferProxy = TransientUploadBufferRange.GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
            _UploadBufferOffsetInBytes = TransientUploadBufferRange.BeginOffsetInBytes;
        }
        else
        {
            _UploadBufferProxy = GetBufferProxy();
            _UploadBufferOffsetInBytes = GetOffsetInBytes();
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
        
        auto D3D12Resource = _UploadBufferProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
        
        auto BufferDataView = GetBufferDataView();
        
        U8* DataPtr = nullptr;
        HRESULT HR = D3D12Resource->Map(0, nullptr, (void**)&DataPtr);
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot map resource";
        
        memcpy(
            DataPtr + _UploadBufferOffsetInBytes,
            BufferDataView.data(),
            BufferDataView.size()
        );
        
        D3D12_RANGE WrittenRange = { _UploadBufferOffsetInBytes, _UploadBufferOffsetInBytes + BufferDataView.size() };
        D3D12Resource->Unmap(0, &WrittenRange);
    }
}
#endif