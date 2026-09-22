#pragma once

#include "Abytek/RHISamplerProxy.hpp"
#include "Abytek/DirectX12/RHIDescriptor.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHISamplerProxy : public A_RHISamplerProxy
    {
    private:
        F_DirectX12RHIDescriptorAllocationProxy _DescriptorRange;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetDescriptorRange() const noexcept
        {
            return _DescriptorRange;
        }
        ABYTEK_FORCE_INLINE auto& InjectDescriptorRange() noexcept
        {
            return _DescriptorRange;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHISamplerProxy);
        void LateBuildSampler_DirectX12();
        virtual void Release() override;
        
    public:
        void AssignDescriptorRange(const F_DirectX12RHIDescriptorAllocationProxy& DescriptorRange);
        
    public:
        DirectX12SharedAPIWrapper::F_D3D12DescriptorDesc GetDescriptorDesc();
    };
}
#endif