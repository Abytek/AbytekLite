#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"


namespace Abytek
{
    struct F_JSONOptions
    {
        TW<F_ReflectionContext> ReflectionContext = F_ReflectionContext::GetGlobal();
        TF_Vector<F_Name> RequiredPropertyTags;
        TF_Map<F_Name, F_Any> Metadata;
        B8 SkipNonDirtyProperties = false;
        void* CDO = nullptr;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        B8 HasDevelopmentBuild = true;
#else
        B8 HasDevelopmentBuild = false;
#endif
    };
}