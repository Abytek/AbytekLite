/**
 * @file RuntimeContainerTypedefs.inl
 * @brief Typedefs for runtime container implementations
 * 
 * This file provides type aliases for runtime containers with default allocators.
 * These typedefs simplify the use of runtime containers by providing more concise
 * type names with default template parameters.
 */

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/RuntimeContainerTemplates.hpp"

#ifndef ABYTEK_RUNTIME_CONTAINER_TYPEDEFS_NAMESPACE
#define ABYTEK_RUNTIME_CONTAINER_TYPEDEFS_NAMESPACE Abytek
#endif


namespace ABYTEK_RUNTIME_CONTAINER_TYPEDEFS_NAMESPACE
{
    /**
     * @brief Default runtime vector instance type using the default allocator
     */
    using A_RuntimeVectorInstance = ContainerTemplates::TA_RuntimeVectorInstance<TF_DefaultAllocator<U8>>;

    /**
     * @brief Template for runtime vector instances of specific types
     * @tparam __F_Item Type of items stored in the runtime vector
     */
    template<typename __F_Item>
    using TF_RuntimeVectorInstance = ContainerTemplates::TF_RuntimeVectorInstance<__F_Item, TF_DefaultAllocator<U8>>;
    
    /**
     * @brief Runtime vector with default allocator
     */
    using F_RuntimeVector = ContainerTemplates::TF_RuntimeVector<TF_DefaultAllocator<U8>>;
}


#undef ABYTEK_RUNTIME_CONTAINER_TYPEDEFS_NAMESPACE