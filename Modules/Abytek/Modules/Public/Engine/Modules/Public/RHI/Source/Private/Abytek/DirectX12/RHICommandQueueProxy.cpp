#include "Abytek/DirectX12/RHICommandQueueProxy.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/RHIContextProxy.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHICommandQueueProxy);
    void F_DirectX12RHICommandQueueProxy::LateBuildCommandQueue_DirectX12()
    {
        auto CommandQueue = GetCommandQueue();
        _Flags = CommandQueue->GetFlags();
        _CommandListType = CommandQueue->GetCommandListType();
    }
    void F_DirectX12RHICommandQueueProxy::Release()
    {
        _D3D12CommandQueue = nullptr;
        _Flags = DirectX12SharedAPIWrapper::E_CommandQueueFlag::NONE;
        _CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::NONE;
        A_RHIContextChildProxy::Release();
    }
 
    void F_DirectX12RHICommandQueueProxy::InitD3D12CommandQueue()
    {
        auto D3D12Device = GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        
        D3D12_COMMAND_QUEUE_DESC D3D12CommandQueueDesc = {};
        D3D12CommandQueueDesc.Flags = DirectX12SharedAPIWrapper::Conversions::CommandQueueFlagToD3D12CommandQueueFlags(_Flags);
        D3D12CommandQueueDesc.Type = DirectX12SharedAPIWrapper::Conversions::CommandListTypeToD3D12CommandListType(_CommandListType);
        D3D12CommandQueueDesc.NodeMask = 1;
        HRESULT HR = D3D12Device->CreateCommandQueue(&D3D12CommandQueueDesc, IID_PPV_ARGS(&_D3D12CommandQueue));   
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Failed to create D3D12CommandQueue";
    }

    void F_DirectX12RHICommandQueueProxy::ExecuteCommandList(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& D3D12CommandList)
    { 
        ID3D12CommandList* D3D12CommandLists[] = { D3D12CommandList.Get() };
        _D3D12CommandQueue->ExecuteCommandLists(1, D3D12CommandLists);
    }
}
#endif