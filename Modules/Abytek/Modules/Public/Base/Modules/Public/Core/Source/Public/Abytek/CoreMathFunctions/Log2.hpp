#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the base-2 logarithm of a 32-bit floating point value
     * 
     * @param A Input value
     * @return Base-2 logarithm of A
     */
    ABYTEK_FORCE_INLINE F32 Log2(F32 A) noexcept
    {
        return std::log2(A);
    }
    
    /**
     * @brief Calculates the base-2 logarithm of a 64-bit floating point value
     * 
     * @param A Input value
     * @return Base-2 logarithm of A
     */
    ABYTEK_FORCE_INLINE F64 Log2(F64 A) noexcept
    {
        return std::log2(A);
    }
}