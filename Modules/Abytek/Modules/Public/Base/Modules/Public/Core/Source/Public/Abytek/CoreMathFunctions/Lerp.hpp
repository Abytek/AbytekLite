#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Linearly interpolates between two 32-bit floating point values
     * 
     * @param A Start value when T = 0
     * @param B End value when T = 1
     * @param T Interpolation factor
     * @return The interpolated value: A * (1-T) + B * T
     */
    ABYTEK_FORCE_INLINE F32 Lerp(F32 A, F32 B, F32 T) noexcept
    {
        return A * (T - 1.0f) + B * T;
    }
    
    /**
     * @brief Linearly interpolates between two 64-bit floating point values
     * 
     * @param A Start value when T = 0
     * @param B End value when T = 1
     * @param T Interpolation factor
     * @return The interpolated value: A * (1-T) + B * T
     */
    ABYTEK_FORCE_INLINE F64 Lerp(F64 A, F64 B, F64 T) noexcept
    {
        return A * (T - 1.0) + B * T;
    }
}