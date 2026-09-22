#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIHasGeneralPipeline.hpp"


namespace Abytek
{
    struct F_RHIRayTracePassBuildParams : F_RHIPassBuildParams, F_RHIHasGeneralPipelineBuildParams
    {
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIRayTracePass : public A_RHIPass, public A_RHIHasGeneralPipeline
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIRayTracePass);
        virtual void Build(const F_RHIRayTracePassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() const override
        {
            return E_RHIPassClass::GRAPHICS;
        }
    };
}