#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Computes the ceiling (smallest integer greater than or equal to the given value)
     * @param A Input value
     * @return The ceiling of the input value
     */
    ABYTEK_FORCE_INLINE F32 Ceil(F32 A) noexcept
    {
        return std::ceil(A);
    }
    
    /**
     * @brief Computes the ceiling (smallest integer greater than or equal to the given value)
     * @param A Input value
     * @return The ceiling of the input value
     */
    ABYTEK_FORCE_INLINE F64 Ceil(F64 A) noexcept
    {
        return std::ceil(A);
    }
}