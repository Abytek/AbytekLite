#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the base-10 logarithm of a 32-bit floating point value
     * 
     * @param A Input value
     * @return Base-10 logarithm of A
     */
    ABYTEK_FORCE_INLINE F32 Log10(F32 A) noexcept
    {
        return std::log10(A);
    }
    
    /**
     * @brief Calculates the base-10 logarithm of a 64-bit floating point value
     * 
     * @param A Input value
     * @return Base-10 logarithm of A
     */
    ABYTEK_FORCE_INLINE F64 Log10(F64 A) noexcept
    {
        return std::log10(A);
    }
}