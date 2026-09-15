#pragma once

#include "Abytek/RHIDeviceChild.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHICommandQueueProxy;
    
    struct F_DirectX12RHIFenceBuildParams
    {
        TW<A_RHIDevice> Device;
        U64 InitialValue = 0;
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIFence : public A_RHIDeviceChild
    {
    private:
        Microsoft::WRL::ComPtr<ID3D12Fence> _D3D12Fence = 0;
        U64 _InitialValue = 0;

    public:
        ABYTEK_FORCE_INLINE const auto& GetD3D12Fence() const noexcept
        {
            return _D3D12Fence;
        }
        ABYTEK_FORCE_INLINE auto& InjectD3D12Fence() noexcept
        {
            return _D3D12Fence;
        }
        ABYTEK_FORCE_INLINE U64 GetInitialValue() const noexcept
        {
            return _InitialValue;
        }
        ABYTEK_FORCE_INLINE auto& InjectInitialValue() noexcept
        {
            return _InitialValue;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHIFence);
        void Build(const F_DirectX12RHIFenceBuildParams& BuildParams);
        virtual void Release() override;

    public:
        void SignalFromCPUSide(U64 NewValue);
        void SignalFromGPUSide(U64 NewValue, const TW_Valid<F_DirectX12RHICommandQueueProxy>& CommandQueueProxy);
        void WaitOnGPUSide(U64 Value, const TW_Valid<F_DirectX12RHICommandQueueProxy>& CommandQueueProxy);
        U64 GetCompletedValue() const;
        B8 IsPassed(U64 MinValue) const;
        void WaitOnCPUSide(U64 MinValue) const;
    };
}
#endif