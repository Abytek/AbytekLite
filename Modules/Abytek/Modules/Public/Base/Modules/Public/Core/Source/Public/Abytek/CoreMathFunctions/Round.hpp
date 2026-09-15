#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Rounds a float to the nearest integer value (as a float)
     * 
     * Uses std::round to round the value to the nearest integer value,
     * returning the result as a floating-point number.
     * 
     * @param A The floating-point value to round
     * @return F32 The rounded value as a float
     */
    ABYTEK_FORCE_INLINE F32 Round(F32 A) noexcept
    {
        return std::round(A);
    }
    
    /**
     * @brief Rounds a double to the nearest integer value (as a double)
     * 
     * Uses std::round to round the value to the nearest integer value,
     * returning the result as a double-precision floating-point number.
     * 
     * @param A The double-precision value to round
     * @return F64 The rounded value as a double
     */
    ABYTEK_FORCE_INLINE F64 Round(F64 A) noexcept
    {
        return std::round(A);
    }
}