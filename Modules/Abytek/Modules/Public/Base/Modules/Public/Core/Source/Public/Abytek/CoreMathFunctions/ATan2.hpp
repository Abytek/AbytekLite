#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Computes the 2-argument arctangent (atan2) of Y/X
     * @param A Y value (numerator)
     * @param B X value (denominator)
     * @return The angle in radians in the range [-π, π], representing the angle between the positive X-axis and the point (B, A)
     */
    ABYTEK_FORCE_INLINE F32 ATan2(F32 A, F32 B) noexcept
    {
        return std::atan2(A, B);
    }
    
    /**
     * @brief Computes the 2-argument arctangent (atan2) of Y/X
     * @param A Y value (numerator)
     * @param B X value (denominator)
     * @return The angle in radians in the range [-π, π], representing the angle between the positive X-axis and the point (B, A)
     */
    ABYTEK_FORCE_INLINE F64 ATan2(F64 A, F64 B) noexcept
    {
        return std::atan2(A, B);
    }
}