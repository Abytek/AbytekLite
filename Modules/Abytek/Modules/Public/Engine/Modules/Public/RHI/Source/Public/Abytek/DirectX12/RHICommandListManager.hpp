#pragma once

#include "Abytek/RHIDeviceChild.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    struct F_DirectX12RHICommandListManagerBuildParams
    {
        TW<A_RHIDevice> Device;
        DirectX12SharedAPIWrapper::E_CommandListType CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::DEFAULT;
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHICommandListManager : public A_RHIDeviceChild
    {
    private:
        DirectX12SharedAPIWrapper::E_CommandListType _CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::NONE;
        TF_ConcurrentQueue<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> _D3D12CommandAllocators;
        TF_ConcurrentQueue<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>> _D3D12CommandListsToPop;
        TF_ConcurrentQueue<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>> _D3D12CommandListsToReset;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetCommandListType() const noexcept
        {
            return _CommandListType;
        }
        ABYTEK_FORCE_INLINE auto& InjectCommandListType() noexcept
        {
            return _CommandListType;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHICommandListManager);
        virtual void Build(const F_DirectX12RHICommandListManagerBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        void PushAllocator(const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& D3D12CommandAllocator);
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> PopAllocator();
        
    public:
        void ResetAllocators();
        
    public:
        void PushList(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& D3D12CommandList);
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> PopList(const Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& D3D12CommandAllocator);
        
    public:
        void ResetLists();
    };
}
#endif