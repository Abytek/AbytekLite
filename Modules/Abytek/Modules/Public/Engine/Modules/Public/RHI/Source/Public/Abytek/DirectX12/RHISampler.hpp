#pragma once

#include "Abytek/RHISampler.hpp"
#include "Abytek/DirectX12/RHIDescriptorAllocation.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHISampler : public A_RHISampler
    {
    private:
        F_DirectX12RHIDescriptorAllocation _DescriptorAllocation;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetDescriptorAllocation() const noexcept
        {
            return _DescriptorAllocation;
        }
        ABYTEK_FORCE_INLINE auto& InjectDescriptorAllocation() noexcept
        {
            return _DescriptorAllocation;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHISampler);
        virtual void Build(const F_RHISamplerBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        void D3D12InitDescriptor();
        void D3D12DeallocateDescriptor();
    };
}
#endif