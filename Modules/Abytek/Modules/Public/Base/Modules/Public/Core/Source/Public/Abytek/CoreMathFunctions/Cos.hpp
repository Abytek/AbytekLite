#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Computes the cosine of an angle
     * @param A Angle in radians
     * @return The cosine of the angle, a value between -1 and 1
     */
    ABYTEK_FORCE_INLINE F32 Cos(F32 A) noexcept
    {
        return std::cos(A);
    }
    
    /**
     * @brief Computes the cosine of an angle
     * @param A Angle in radians
     * @return The cosine of the angle, a value between -1 and 1
     */
    ABYTEK_FORCE_INLINE F64 Cos(F64 A) noexcept
    {
        return std::cos(A);
    }
}