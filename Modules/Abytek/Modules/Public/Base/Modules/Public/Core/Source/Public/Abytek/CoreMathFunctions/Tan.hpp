#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the tangent of a 32-bit floating point value.
     * @param A The angle in radians.
     * @return The tangent of the input angle.
     */
    ABYTEK_FORCE_INLINE F32 Tan(F32 A) noexcept
    {
        return std::tan(A);
    }
    /**
     * @brief Calculates the tangent of a 64-bit floating point value.
     * @param A The angle in radians.
     * @return The tangent of the input angle.
     */
    ABYTEK_FORCE_INLINE F64 Tan(F64 A) noexcept
    {
        return std::tan(A);
    }
}