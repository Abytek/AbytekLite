#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates e^x for a 32-bit floating point value
     * 
     * @param A Input value
     * @return e raised to the power of A
     */
    ABYTEK_FORCE_INLINE F32 Exponential(F32 A) noexcept
    {
        return std::exp(A);
    }
    
    /**
     * @brief Calculates e^x for a 64-bit floating point value
     * 
     * @param A Input value
     * @return e raised to the power of A
     */
    ABYTEK_FORCE_INLINE F64 Exponential(F64 A) noexcept
    {
        return std::exp(A);
    }
}