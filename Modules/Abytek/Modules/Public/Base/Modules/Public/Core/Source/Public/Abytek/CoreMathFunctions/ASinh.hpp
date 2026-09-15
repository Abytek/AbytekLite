#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the hyperbolic arc sine of a 32-bit floating point value.
     * @param A The input value.
     * @return The hyperbolic arc sine of A.
     */
    ABYTEK_FORCE_INLINE F32 ASinh(F32 A) noexcept
    {
        return std::asinh(A);
    }
    /**
     * @brief Calculates the hyperbolic arc sine of a 64-bit floating point value.
     * @param A The input value.
     * @return The hyperbolic arc sine of A.
     */
    ABYTEK_FORCE_INLINE F64 ASinh(F64 A) noexcept
    {
        return std::asinh(A);
    }
}