#pragma once

#include "Abytek/RHITemplateRuntime.hpp"


namespace Abytek
{
    struct F_RHIBindGroupTemplateRuntimeBuildParams : F_RHITemplateRuntimeBuildParams
    {
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIBindGroupTemplateRuntime : public A_RHITemplateRuntime
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIBindGroupTemplateRuntime);
        virtual void Build(const F_RHIBindGroupTemplateRuntimeBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual void FinalizeActivation() override;
        
    protected:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;
    };
}