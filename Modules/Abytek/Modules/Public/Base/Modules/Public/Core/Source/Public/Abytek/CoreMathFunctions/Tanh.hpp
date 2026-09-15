#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the hyperbolic tangent of a 32-bit floating point value.
     * @param A The value to calculate the hyperbolic tangent of.
     * @return The hyperbolic tangent of the input value.
     */
    ABYTEK_FORCE_INLINE F32 Tanh(F32 A) noexcept
    {
        return std::tanh(A);
    }
    /**
     * @brief Calculates the hyperbolic tangent of a 64-bit floating point value.
     * @param A The value to calculate the hyperbolic tangent of.
     * @return The hyperbolic tangent of the input value.
     */
    ABYTEK_FORCE_INLINE F64 Tanh(F64 A) noexcept
    {
        return std::tanh(A);
    }
}