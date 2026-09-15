#pragma once

#include "Abytek/RHIResourceView.hpp"
#include "Abytek/DirectX12/RHIDescriptorAllocation.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIResourceView : public A_RHIResourceView
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
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIResourceView);
        virtual void Release() override;

    public:
        virtual void Build(const F_RHIBufferViewBuildParams& BuildParams) override;
        virtual void Build(const F_RHITextureViewBuildParams& BuildParams) override;
        virtual void Build(const F_RHIRTASViewBuildParams& BuildParams) override;

    public:
        void BuildWithoutDescriptor(const F_RHIBufferViewBuildParams& BuildParams);
        void BuildWithoutDescriptor(const F_RHITextureViewBuildParams& BuildParams);
        void BuildWithoutDescriptor(const F_RHIRTASViewBuildParams& BuildParams);
        
    protected:
        virtual void Build(const F_RHIResourceViewBuildParamsBase& BuildParams);
        
    public:
        void D3D12AllocateDescriptor();
        void D3D12InitDescriptor(B8 ShouldReallocate = true);
        void D3D12DeallocateDescriptor();
        
    public:
        void AppendSubresourceBindingSet(F_DirectX12RHISubresourceBindingSet& Result);
    };
}
#endif