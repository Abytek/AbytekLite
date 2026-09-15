#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the arc sine of a 32-bit floating point value.
     * @param A The input value in range [-1, 1].
     * @return The angle in radians whose sine is A, in range [-π/2, π/2].
     */
    ABYTEK_FORCE_INLINE F32 ASin(F32 A) noexcept
    {
        return std::asin(A);
    }
    /**
     * @brief Calculates the arc sine of a 64-bit floating point value.
     * @param A The input value in range [-1, 1].
     * @return The angle in radians whose sine is A, in range [-π/2, π/2].
     */
    ABYTEK_FORCE_INLINE F64 ASin(F64 A) noexcept
    {
        return std::asin(A);
    }
}