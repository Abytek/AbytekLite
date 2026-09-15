#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the natural logarithm of a 32-bit floating point value
     * 
     * @param A Input value
     * @return Natural logarithm of A
     */
    ABYTEK_FORCE_INLINE F32 LogE(F32 A) noexcept
    {
        return std::log(A);
    }
    
    /**
     * @brief Calculates the natural logarithm of a 64-bit floating point value
     * 
     * @param A Input value
     * @return Natural logarithm of A
     */
    ABYTEK_FORCE_INLINE F64 LogE(F64 A) noexcept
    {
        return std::log(A);
    }
}