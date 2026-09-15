#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/DirectX12/RHIDescriptorAllocation.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    struct F_DirectX12RHIDescriptorManagerBuildParams : F_RHIContextChildBuildParams
    {
        DirectX12SharedAPIWrapper::E_DescriptorHeapType DescriptorHeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::NONE;
        B8 IsShaderVisible = false;
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIDescriptorManager : public A_RHIContextChild
    {
    private:
        DirectX12SharedAPIWrapper::E_DescriptorHeapType _DescriptorHeapType;
        B8 _IsShaderVisible = false;
        
        F_AutoScaleLinearAllocationDistributor _Distributor;
        
    public:
        ABYTEK_FORCE_INLINE auto GetDescriptorHeapType() const noexcept
        {
            return _DescriptorHeapType;
        }
        ABYTEK_FORCE_INLINE auto& InjectDescriptorHeapType() noexcept
        {
            return _DescriptorHeapType;
        }
        ABYTEK_FORCE_INLINE auto IsShaderVisible() const noexcept
        {
            return _IsShaderVisible;
        }
        ABYTEK_FORCE_INLINE auto& InjectIsShaderVisible() noexcept
        {
            return _IsShaderVisible;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetDistributor() const noexcept
        {
            return _Distributor;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHIDescriptorManager);
        void Build(const F_DirectX12RHIDescriptorManagerBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        F_DirectX12RHIDescriptorAllocation Allocate(U32 Num = 1);
        void Deallocate(const F_DirectX12RHIDescriptorAllocation& Allocation);
        
    protected:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;
        
    public:
        virtual B8 UseStrictMode() const override
        {
            return false;
        }
    };
}
#endif