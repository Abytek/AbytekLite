#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Clamps a float value to the range [0.0, 1.0]
     * 
     * Ensures the value is between 0.0 and 1.0 inclusive.
     * Values below 0.0 are clamped to 0.0, values above 1.0 are clamped to 1.0.
     * 
     * @param A The floating-point value to saturate
     * @return F32 The saturated value in range [0.0, 1.0]
     */
    ABYTEK_FORCE_INLINE F32 Saturate(F32 A) noexcept
    {
        if(A < 0.0f)
        {
            return 0.0f;
        }
        if(A > 1.0f)
        {
            return 1.0f;
        }
        return A;
    }
    
    /**
     * @brief Clamps a double value to the range [0.0, 1.0]
     * 
     * Ensures the value is between 0.0 and 1.0 inclusive.
     * Values below 0.0 are clamped to 0.0, values above 1.0 are clamped to 1.0.
     * 
     * @param A The double-precision value to saturate
     * @return F64 The saturated value in range [0.0, 1.0]
     */
    ABYTEK_FORCE_INLINE F64 Saturate(F64 A) noexcept
    {
        if(A < 0.0)
        {
            return 0.0;
        }
        if(A > 1.0)
        {
            return 1.0;
        }
        return A;
    }
}