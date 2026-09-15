#pragma once

#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIConstantDataManager.hpp"
#include "Abytek/DirectX12/RHIDescriptor.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"
#include "Abytek/DirectX12/RHIDescriptorAllocation.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"
#include "Abytek/DirectX12Shared/RHIPipelineStateTemplate.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    struct ABYTEK_ENGINE_RHI_API H_DirectX12RHIVertexBufferBinding
    {
        static void AppendSubresourceBindingSet(const F_RHIVertexBufferBinding& VertexBufferBinding, F_DirectX12RHISubresourceBindingSet& Result);
    };
    struct ABYTEK_ENGINE_RHI_API H_DirectX12RHIIndexBufferBinding
    {
        static void AppendSubresourceBindingSet(const F_RHIIndexBufferBinding& IndexBufferBinding, F_DirectX12RHISubresourceBindingSet& Result);
    };
    
    struct F_DirectX12RHIBindGroupAllocationProxy
    {
        struct F_DescriptorBinding
        {
            F_DirectX12RHIDescriptorRange DescriptorRange;
        };
        struct F_UniformDataBinding
        {
            F_RHIConstantDataRangeProxy ConstantDataRangeProxy;
        };

        // For bindings
        TF_SmallVector<F_DescriptorBinding, 2> DescriptorBindings;
        TF_SmallVector<F_UniformDataBinding, 2> UniformDataBindings;
    };
    struct F_DirectX12RHIBindGroupAllocation
    {
        struct F_DescriptorBinding
        {
            F_DirectX12RHIDescriptorAllocation DescriptorAllocation;
        };
        struct F_UniformDataBinding
        {
            F_RHIConstantDataRange ConstantDataRange;
        };

        // For bindings
        TF_SmallVector<F_DescriptorBinding, 2> DescriptorBindings;
        TF_SmallVector<F_UniformDataBinding, 2> UniformDataBindings;
        
        F_DirectX12RHIBindGroupAllocationProxy ConvertToProxy() const
        {
            F_DirectX12RHIBindGroupAllocationProxy Result;
            Result.DescriptorBindings.reserve(DescriptorBindings.size());
            Result.UniformDataBindings.reserve(UniformDataBindings.size());
            for (const auto& DescriptorBinding : DescriptorBindings)
            {
                F_DirectX12RHIBindGroupAllocationProxy::F_DescriptorBinding Proxy;
                Proxy.DescriptorRange = DescriptorBinding.DescriptorAllocation.ConvertToRange(); 
                Result.DescriptorBindings.emplace_back(Proxy);
            }
            for (const auto& UniformDataBinding : UniformDataBindings)
            {
                F_DirectX12RHIBindGroupAllocationProxy::F_UniformDataBinding Proxy;
                Proxy.ConstantDataRangeProxy = UniformDataBinding.ConstantDataRange.ConvertToProxy(); 
                Result.UniformDataBindings.emplace_back(Proxy);
            }
            return ABYTEK_MOVE(Result);
        }
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIBindGroup : public A_RHIBindGroup
    {
    private:
#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
        F_AtomicFlag _IsCommitted;
#endif

        F_DirectX12RHIBindGroupAllocation _Allocation;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetAllocation() const noexcept
        {
            return _Allocation;
        }
        ABYTEK_FORCE_INLINE auto& InjectAllocation() noexcept
        {
            return _Allocation;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIBindGroup);
        void Build(const F_RHIBindGroupBuildParams& BuildParams) override;
        void Release() override;

    public:
        void Commit() override;
        void EncodeData(void* OutData) override; 
        
    public:
        F_RHIResourceViewBindlessRange GetResourceViewBindlessRange(U32 Index) const override;
        F_RHISamplerBindlessRange GetSamplerBindlessRange(U32 Index) const override;
        
    private:
        void _ReallocateIfNeeded();
        void _SendCommitToProxy();
        void _CopyDescriptors();
        void _UploadConstantData();
        
    public:
        void AppendSubresourceBindingSet(F_DirectX12RHISubresourceBindingSet& Result);
    };
}
#endif