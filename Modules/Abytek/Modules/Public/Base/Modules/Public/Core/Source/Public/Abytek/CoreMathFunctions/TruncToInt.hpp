#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Truncates a 32-bit floating point value to a 32-bit signed integer.
     * @param A The floating point value to truncate.
     * @return The truncated integer value.
     */
    ABYTEK_FORCE_INLINE I32 TruncToInt(F32 A) noexcept
    {
        return static_cast<I32>(A);
    }
    /**
     * @brief Truncates a 64-bit floating point value to a 64-bit signed integer.
     * @param A The floating point value to truncate.
     * @return The truncated integer value.
     */
    ABYTEK_FORCE_INLINE I64 TruncToInt(F64 A) noexcept
    {
        return static_cast<I64>(A);
    }
}