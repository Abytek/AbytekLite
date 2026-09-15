#pragma once

#include "Abytek/RHITemplateRuntime.hpp"


namespace Abytek
{
    class A_RHIBindGroupTemplateRuntime;
    
    struct F_RHIPipelineStateTemplateRuntimeBuildParams : F_RHITemplateRuntimeBuildParams
    {
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIPipelineStateTemplateRuntime : public A_RHITemplateRuntime
    {
    private:
        TF_Vector<TS<A_RHIBindGroupTemplateRuntime>> _BindGroupTemplateRuntimes;

    public:
        ABYTEK_FORCE_INLINE const auto& GetBindGroupTemplateRuntimes() const noexcept
        {
            return _BindGroupTemplateRuntimes;
        }
        ABYTEK_FORCE_INLINE auto& InjectBindGroupTemplateRuntimes() noexcept
        {
            return _BindGroupTemplateRuntimes;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIPipelineStateTemplateRuntime);
        virtual void Build(const F_RHIPipelineStateTemplateRuntimeBuildParams& BuildParams);
        virtual void Release() override;

    protected:
        virtual void FinalizeActivation() override;
        
    protected:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;
    };
}