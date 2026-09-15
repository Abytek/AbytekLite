#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    class A_RHIContext;
    class A_RHIDevice;
    
    using F_RHIBindlessId = U32;
    static constexpr F_RHIBindlessId INVALID_RHI_BINDLESS_ID = ~F_RHIBindlessId(0);
    
    struct F_RHIBindlessRange
    {
        U32 BeginIndex = 0;
        U32 EndIndex = 0;
        
        ABYTEK_FORCE_INLINE U32 GetSize() const noexcept
        {
            return EndIndex - BeginIndex;
        }
    };
}

namespace boost
{
}