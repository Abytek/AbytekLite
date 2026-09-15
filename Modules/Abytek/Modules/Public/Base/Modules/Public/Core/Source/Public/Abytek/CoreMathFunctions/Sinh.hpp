#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the hyperbolic sine of a float value
     * 
     * Uses the standard library sinh function to compute the hyperbolic sine.
     * 
     * @param A Input value
     * @return F32 Hyperbolic sine of the input value
     */
    ABYTEK_FORCE_INLINE F32 Sinh(F32 A) noexcept
    {
        return std::sinh(A);
    }
    
    /**
     * @brief Calculates the hyperbolic sine of a double value
     * 
     * Uses the standard library sinh function to compute the hyperbolic sine.
     * 
     * @param A Input value
     * @return F64 Hyperbolic sine of the input value
     */
    ABYTEK_FORCE_INLINE F64 Sinh(F64 A) noexcept
    {
        return std::sinh(A);
    }
}