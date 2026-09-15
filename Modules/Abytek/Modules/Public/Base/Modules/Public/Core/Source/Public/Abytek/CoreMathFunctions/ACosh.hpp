#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the hyperbolic arc cosine of a 32-bit floating point value.
     * @param A The input value (must be >= 1).
     * @return The hyperbolic arc cosine of A.
     */
    ABYTEK_FORCE_INLINE F32 ACosh(F32 A) noexcept
    {
        return std::acosh(A);
    }
    /**
     * @brief Calculates the hyperbolic arc cosine of a 64-bit floating point value.
     * @param A The input value (must be >= 1).
     * @return The hyperbolic arc cosine of A.
     */
    ABYTEK_FORCE_INLINE F64 ACosh(F64 A) noexcept
    {
        return std::acosh(A);
    }
}