
#include "Abytek/Engine.Core.prerequisites.hpp"

#ifndef ABYTEK_USE_TRANSIENT_ALLOCATOR_TYPEDEFS_NAMESPACE
#error "unknown use-transient allocator typedefs namespace"
#endif

namespace ABYTEK_USE_TRANSIENT_ALLOCATOR_TYPEDEFS_NAMESPACE
{
    template<typename __F>
    using TF_DefaultAllocator = TF_StaticTransientAllocator<__F, DefaultTransientMemoryParam>;
    using F_DefaultObjectManagement = WithCrtAllocator::F_DefaultObjectManagement;
}

#define ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE ABYTEK_USE_TRANSIENT_ALLOCATOR_TYPEDEFS_NAMESPACE
#include "Abytek/ContainerTypedefs.inl"

#define ABYTEK_OBJECT_TYPEDEFS_NAMESPACE ABYTEK_USE_TRANSIENT_ALLOCATOR_TYPEDEFS_NAMESPACE
#include "Abytek/ObjectTypedefs.inl"

#undef ABYTEK_USE_TRANSIENT_ALLOCATOR_TYPEDEFS_NAMESPACE