#pragma once

#include "Abytek/RHIDeviceChild.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHIFence;
    class F_DirectX12RHICommandQueueProxy;
    
    struct F_DirectX12RHISynchronizerBuildParams
    {
        TW<A_RHIDevice> Device;
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHISynchronizer : public A_RHIDeviceChild
    {
    private:
        TU<F_DirectX12RHIFence> _Fence;
        AU64 _NextAllocatedValue = 1;
        U64 _LastSignaledValue = 0;
        F_YieldCriticalSection _SignalCriticalSection;
        
    public:
        ABYTEK_FORCE_INLINE auto GetFence() const noexcept
        {
            return _Fence.Weak();
        }
        ABYTEK_FORCE_INLINE auto& InjectFence() noexcept
        {
            return _Fence;
        }
        ABYTEK_FORCE_INLINE U64 GetNextAllocatedValue() const noexcept
        {
            return _NextAllocatedValue.load(boost::memory_order_acquire);
        }
        ABYTEK_FORCE_INLINE auto& InjectNextAllocatedValue() noexcept
        {
            return _NextAllocatedValue;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHISynchronizer);
        void Build(const F_DirectX12RHISynchronizerBuildParams& BuildParams);
        virtual void Release() override;

    public:
        U64 AllocateNewValue();
        void SignalFromGPUSide(U64 Value, const TW_Valid<F_DirectX12RHICommandQueueProxy>& CommandQueueProxy);
        void WaitOnCPUSide(U64 Value);
        void WaitOnGPUSide(U64 Value, const TW_Valid<F_DirectX12RHICommandQueueProxy>& CommandQueueProxy);
    };
}
#endif