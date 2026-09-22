#include "Abytek/DirectX12/RHIDescriptor.hpp"
#include "Abytek/DirectX12/RHIDescriptorManagerProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    D3D12_CPU_DESCRIPTOR_HANDLE F_DirectX12RHIDescriptorAllocationProxy::GetCPUHandle(B8 IsShaderVisible) const noexcept
    {
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> D3D12DescriptorHeap;
        if (IsShaderVisible)
        {
            D3D12DescriptorHeap = ManagerProxy->GetD3D12DescriptorHeap_ShaderVisible();
        }
        else
        {
            D3D12DescriptorHeap = ManagerProxy->GetD3D12DescriptorHeap_NonShaderVisible();
        }
        
        D3D12_CPU_DESCRIPTOR_HANDLE Result = D3D12DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        Result.ptr += static_cast<Sz>(ManagerProxy->GetD3D12DescriptorHandleStride()) * static_cast<Sz>(BeginOffset);
        return Result;
    }
    D3D12_GPU_DESCRIPTOR_HANDLE F_DirectX12RHIDescriptorAllocationProxy::GetGPUHandle() const noexcept
    {
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> D3D12DescriptorHeap = ManagerProxy->GetD3D12DescriptorHeap_ShaderVisible();
        
        D3D12_GPU_DESCRIPTOR_HANDLE Result = D3D12DescriptorHeap->GetGPUDescriptorHandleForHeapStart();
        Result.ptr += static_cast<Sz>(ManagerProxy->GetD3D12DescriptorHandleStride()) * static_cast<Sz>(BeginOffset);
        return Result;
    }
}
#endif