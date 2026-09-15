#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/DirectX12/RHICommandQueue.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHICommandQueueProxy : public A_RHIContextChildProxy
    {
    private:
        DirectX12SharedAPIWrapper::E_CommandListType _CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::NONE;
        DirectX12SharedAPIWrapper::E_CommandQueueFlag _Flags = DirectX12SharedAPIWrapper::E_CommandQueueFlag::NONE;
        
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> _D3D12CommandQueue = nullptr;
        
    public:
        ABYTEK_FORCE_INLINE auto GetCommandQueue() const noexcept
        {
            return GetContextChild().FastCast<F_DirectX12RHICommandQueue>();
        }
        
        ABYTEK_FORCE_INLINE const auto& GetCommandListType() const noexcept
        {
            return _CommandListType;
        }
        ABYTEK_FORCE_INLINE auto& InjectCommandListType() noexcept
        {
            return _CommandListType;
        }
        ABYTEK_FORCE_INLINE const auto& GetFlags() const noexcept
        {
            return _Flags;
        }
        ABYTEK_FORCE_INLINE auto& InjectFlags() noexcept
        {
            return _Flags;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetD3D12CommandQueue() const noexcept
        {
            return _D3D12CommandQueue;
        }
        ABYTEK_FORCE_INLINE auto& InjectD3D12CommandQueue() noexcept
        {
            return _D3D12CommandQueue;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHICommandQueueProxy);
        void LateBuildCommandQueue_DirectX12();
        virtual void Release() override;
        
    public:
        void InitD3D12CommandQueue();
        
    public:
        void ExecuteCommandList(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& D3D12CommandList);
    };
}
#endif