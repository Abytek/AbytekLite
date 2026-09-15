#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/TaggedPath.hpp"


namespace Abytek
{
    class F_RenderRegistry;
    
    struct F_RenderCoreRHIConfig
    {
        E_RHIAPI API = E_RHIAPI::NONE;
        F_RHIFeatureSupports FeatureSupports;
    };
    struct F_RenderRegistryBuildParams
    {
        F_RenderCoreRHIConfig RHIConfig;
        TF_Vector<TS<F_RenderRegistry>> Dependencies;
    };
}