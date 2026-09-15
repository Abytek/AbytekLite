#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the arc cosine of a 32-bit floating point value.
     * @param A The input value in range [-1, 1].
     * @return The angle in radians whose cosine is A, in range [0, π].
     */
    ABYTEK_FORCE_INLINE F32 ACos(F32 A) noexcept
    {
        return std::acos(A);
    }
    /**
     * @brief Calculates the arc cosine of a 64-bit floating point value.
     * @param A The input value in range [-1, 1].
     * @return The angle in radians whose cosine is A, in range [0, π].
     */
    ABYTEK_FORCE_INLINE F64 ACos(F64 A) noexcept
    {
        return std::acos(A);
    }
}