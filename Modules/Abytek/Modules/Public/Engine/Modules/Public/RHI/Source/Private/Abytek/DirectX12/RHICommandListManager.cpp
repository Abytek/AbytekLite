#include "Abytek/DirectX12/RHICommandListManager.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHICommandListManager);

    void F_DirectX12RHICommandListManager::Build(const F_DirectX12RHICommandListManagerBuildParams& BuildParams)
    {
        A_RHIDeviceChild::Build(BuildParams.Device);
        
        _CommandListType = BuildParams.CommandListType;
    }
    void F_DirectX12RHICommandListManager::Release()
    {
        {
            Microsoft::WRL::ComPtr<ID3D12CommandAllocator> D3D12CommandAllocator = nullptr;
            while (_D3D12CommandAllocators.TryPop(D3D12CommandAllocator));
        }
        {
            Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> D3D12CommandList = nullptr;
            while (_D3D12CommandListsToPop.TryPop(D3D12CommandList));
        }
        _D3D12CommandAllocators = {};
        _D3D12CommandListsToPop = {};
        _D3D12CommandListsToReset = {};
        _CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::NONE;
        A_RHIDeviceChild::Release();
    }

    void F_DirectX12RHICommandListManager::PushAllocator(const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& D3D12CommandAllocator)
    {
        _D3D12CommandAllocators.Push(D3D12CommandAllocator);
    }
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> F_DirectX12RHICommandListManager::PopAllocator()
    {
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> D3D12CommandAllocator = nullptr;
        while (_D3D12CommandAllocators.TryPop(D3D12CommandAllocator))
        {
            return D3D12CommandAllocator;
        }
        
        auto D3D12Device = GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        D3D12_COMMAND_LIST_TYPE D3D12CommandListType = DirectX12SharedAPIWrapper::Conversions::CommandListTypeToD3D12CommandListType(_CommandListType);
        HRESULT HR = D3D12Device->CreateCommandAllocator(D3D12CommandListType, IID_PPV_ARGS(&D3D12CommandAllocator));
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot create D3D12CommandAllocator";
        return D3D12CommandAllocator;
    }
    void F_DirectX12RHICommandListManager::ResetAllocators()
    {
        TF_SmallVector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>, 32> CachedCommandAllocators;
        
        {
            Microsoft::WRL::ComPtr<ID3D12CommandAllocator> D3D12CommandAllocator = nullptr;
            while (_D3D12CommandAllocators.TryPop(D3D12CommandAllocator))
            {
                D3D12CommandAllocator->Reset();
                CachedCommandAllocators.push_back(D3D12CommandAllocator);
            }
        }
        
        for (auto D3D12CommandAllocator : CachedCommandAllocators)
        {
            _D3D12CommandAllocators.Push(D3D12CommandAllocator);
        }
    }

    void F_DirectX12RHICommandListManager::PushList(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& D3D12CommandList)
    {
        _D3D12CommandListsToReset.Push(D3D12CommandList);
    }
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> F_DirectX12RHICommandListManager::PopList(const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& D3D12CommandAllocator)
    {
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> D3D12CommandList = nullptr;
        while (_D3D12CommandListsToPop.TryPop(D3D12CommandList))
        { 
            D3D12CommandList->Reset(D3D12CommandAllocator.Get(), nullptr);
            return D3D12CommandList;
        }
        
        auto D3D12Device = GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        D3D12_COMMAND_LIST_TYPE D3D12CommandListType = DirectX12SharedAPIWrapper::Conversions::CommandListTypeToD3D12CommandListType(_CommandListType);
        HRESULT HR = D3D12Device->CreateCommandList(1, D3D12CommandListType, D3D12CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&D3D12CommandList));
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot create D3D12CommandList";
        return D3D12CommandList;
    }
    void F_DirectX12RHICommandListManager::ResetLists()
    {
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> D3D12CommandList = nullptr;
        while (_D3D12CommandListsToReset.TryPop(D3D12CommandList))
        {
            _D3D12CommandListsToPop.Push(D3D12CommandList);
        }
    }
}
#endif