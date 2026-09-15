#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates floor for a 32-bit floating point value
     * 
     * @param A Input value
     * @return Largest integer value not greater than A
     */
    ABYTEK_FORCE_INLINE F32 Floor(F32 A) noexcept
    {
        return std::floor(A);
    }
    
    /**
     * @brief Calculates floor for a 64-bit floating point value
     * 
     * @param A Input value
     * @return Largest integer value not greater than A
     */
    ABYTEK_FORCE_INLINE F64 Floor(F64 A) noexcept
    {
        return std::floor(A);
    }
}