#pragma once

#include "Abytek/SDK/Core.prerequisites.pch.hpp"


namespace Abytek::SDK
{
    struct F_PrimitiveExternalId
    {
        U64 Value = 0; 
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return Value != 0;
        }
        explicit ABYTEK_FORCE_INLINE operator B8() const noexcept
        {
            return IsValid();
        }
    };
    struct F_PrimitiveBuildParams
    {
        F_PrimitiveExternalId ExternalId;
    };
    struct F_PrimitiveTag
    {
    };
}