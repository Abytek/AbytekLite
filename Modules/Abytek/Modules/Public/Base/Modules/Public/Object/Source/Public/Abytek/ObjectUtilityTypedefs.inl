
#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/ObjectUtilityTemplates.hpp"
#include "Abytek/StandardObjectManagement.hpp"

#ifndef ABYTEK_OBJECT_UTILITY_TYPEDEFS_NAMESPACE
#define ABYTEK_OBJECT_UTILITY_TYPEDEFS_NAMESPACE Abytek
#endif


namespace ABYTEK_OBJECT_UTILITY_TYPEDEFS_NAMESPACE
{
    template<class __F_PassedObject>
    using TF_UniqueObjectSet = Abytek::ObjectUtilityTemplates::TF_UniqueObjectSet<
        __F_PassedObject,
        TF_DefaultAllocator<Abytek::A_Object>,
        F_DefaultObjectConfig
    >;
    
    using F_GBMObjectConfig = Abytek::ObjectUtilityTemplates::TF_GBMObjectConfig<
        TF_DefaultAllocator<Abytek::A_Object>,
        F_DefaultObjectConfig
    >;
    
    using A_GBMObject = Abytek::ObjectUtilityTemplates::TA_GBMObject<
        TF_DefaultAllocator<Abytek::A_Object>,
        F_DefaultObjectConfig
    >;
    using F_GBMObjectContainer = Abytek::ObjectUtilityTemplates::TF_GBMObjectContainer<
        TF_DefaultAllocator<Abytek::A_Object>,
        F_DefaultObjectConfig
    >;
}


#undef ABYTEK_OBJECT_UTILITY_TYPEDEFS_NAMESPACE