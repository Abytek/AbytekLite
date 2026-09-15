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
        const auto& TransientReadbackBufferRange = CastedPass->GetTransientReadbackBufferRange();
        if (TransientReadbackBufferRange)
        {
            _ReadbackBufferProxy = TransientReadbackBufferRange.GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
            _ReadbackBufferOffsetInBytes = TransientReadbackBufferRange.BeginOffsetInBytes;
        }
        else
        {
            _ReadbackBufferProxy = GetBufferProxy();
            _ReadbackBufferOffsetInBytes = GetOffsetInBytes();
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
        
        auto D3D12Resource = _ReadbackBufferProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
    
        auto SizeInBytes = GetSizeInBytes();
        F_RHIBufferData BufferData;
        BufferData.resize(SizeInBytes);
        
        D3D12_RANGE D3D12ReadRange = { _ReadbackBufferOffsetInBytes, _ReadbackBufferOffsetInBytes + SizeInBytes };
        U8* DataPtr = nullptr;
        HRESULT HR = D3D12Resource->Map(0, &D3D12ReadRange, (void**)&DataPtr);
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot map resource";
        
        memcpy(
            BufferData.data(),
            DataPtr + _ReadbackBufferOffsetInBytes,
            SizeInBytes
        );
        
        GetCallback()(BufferData);
        
        D3D12Resource->Unmap(0, nullptr);
    }
}
#endif