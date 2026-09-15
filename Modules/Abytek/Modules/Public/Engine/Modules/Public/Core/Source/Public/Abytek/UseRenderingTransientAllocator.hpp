#pragma once

#include "Abytek/TransientAllocator.hpp"


#define ABYTEK_USE_TRANSIENT_ALLOCATOR_TYPEDEFS_NAMESPACE Abytek::WithRenderAllocator

namespace ABYTEK_USE_TRANSIENT_ALLOCATOR_TYPEDEFS_NAMESPACE
{
    static constexpr E_TransientMemoryParam DefaultTransientMemoryParam = E_TransientMemoryParam::RENDER;
}

#include "Abytek/UseTransientAllocator.inl"