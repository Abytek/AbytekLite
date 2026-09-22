#pragma once

#include "Abytek/RHIResourceViewProxy.hpp"
#include "Abytek/DirectX12/RHIDescriptor.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIResourceViewProxy : public A_RHIResourceViewProxy
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
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIResourceViewProxy);
        void LateBuildResourceView_DirectX12();
        void LateBuildBufferView_DirectX12();
        void LateBuildTextureView_DirectX12();
        void LateBuildRTASView_DirectX12();
        virtual void Release() override;
        
    public:
        void AssignDescriptorRange(const F_DirectX12RHIDescriptorAllocationProxy& DescriptorRange);
        
    public:
        DirectX12SharedAPIWrapper::F_D3D12DescriptorDesc GetDescriptorDesc();
    };
}
#endif