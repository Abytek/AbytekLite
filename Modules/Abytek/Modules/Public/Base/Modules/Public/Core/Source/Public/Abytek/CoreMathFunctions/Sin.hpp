#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the sine of a float angle in radians
     * 
     * Uses the standard library sin function to compute the sine of the input angle.
     * 
     * @param A Angle in radians
     * @return F32 Sine of the angle
     */
    ABYTEK_FORCE_INLINE F32 Sin(F32 A) noexcept
    {
        return std::sin(A);
    }
    
    /**
     * @brief Calculates the sine of a double angle in radians
     * 
     * Uses the standard library sin function to compute the sine of the input angle.
     * 
     * @param A Angle in radians
     * @return F64 Sine of the angle
     */
    ABYTEK_FORCE_INLINE F64 Sin(F64 A) noexcept
    {
        return std::sin(A);
    }
}