#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIHasGeneralPipeline.hpp"
#include "Abytek/RHIIndirectConfig.hpp"


namespace Abytek
{
    struct F_RHIDispatchComputePassBuildParams : F_RHIPassBuildParams, F_RHIHasGeneralPipelineBuildParams
    {
        F_Vector3_U32 NumThreadGroups = F_Vector3_U32::Zero();
        
        TF_Optional<F_RHIIndirectConfig> Indirect;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIDispatchComputePass : public A_RHIPass, public A_RHIHasGeneralPipeline
    {
    private:
        F_Vector3_U32 _NumThreadGroups = F_Vector3_U32::Zero();
        
        TF_Optional<F_RHIIndirectConfig> _Indirect;

    public:
        ABYTEK_FORCE_INLINE const auto& GetNumThreadGroups() const noexcept
        {
            return _NumThreadGroups;
        }
        ABYTEK_FORCE_INLINE auto& InjectNumThreadGroups() noexcept
        {
            return _NumThreadGroups;
        }

        ABYTEK_FORCE_INLINE const auto& GetIndirect() const noexcept
        {
            return _Indirect;
        }
        ABYTEK_FORCE_INLINE auto& InjectIndirect() noexcept
        {
            return _Indirect;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIDispatchComputePass);
        virtual void Build(const F_RHIDispatchComputePassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() const override
        {
            return E_RHIPassClass::COMPUTE;
        }
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;
    };
}