#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the square root of a 32-bit floating point value.
     * @param A The value to calculate the square root of.
     * @return The square root of the input value.
     */
    ABYTEK_FORCE_INLINE F32 SquareRoot(F32 A) noexcept
    {
        return std::sqrt(A);
    }
    /**
     * @brief Calculates the square root of a 64-bit floating point value.
     * @param A The value to calculate the square root of.
     * @return The square root of the input value.
     */
    ABYTEK_FORCE_INLINE F64 SquareRoot(F64 A) noexcept
    {
        return std::sqrt(A);
    }
}