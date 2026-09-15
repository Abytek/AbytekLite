#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"

namespace Abytek
{
    /**
     * @brief Computes the arctangent (inverse tangent) of a value
     * @param A Input value
     * @return The arctangent of the input value in radians, in the range [-π/2, π/2]
     */
    ABYTEK_FORCE_INLINE F32 ATan(F32 A) noexcept
    {
        return std::atan(A);
    }
    
    /**
     * @brief Computes the arctangent (inverse tangent) of a value
     * @param A Input value
     * @return The arctangent of the input value in radians, in the range [-π/2, π/2]
     */
    ABYTEK_FORCE_INLINE F64 ATan(F64 A) noexcept
    {
        return std::atan(A);
    }
}