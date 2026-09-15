#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIHasGeneralPipeline.hpp"


namespace Abytek
{
    struct F_RHIWorkGraphPassBuildParams : F_RHIPassBuildParams, F_RHIHasGeneralPipelineBuildParams
    {
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIWorkGraphPass : public A_RHIPass, public A_RHIHasGeneralPipeline
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIWorkGraphPass);
        virtual void Build(const F_RHIWorkGraphPassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() override
        {
            return E_RHIPassClass::COMPUTE;
        }
    };
}