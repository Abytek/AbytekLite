#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Computes the hyperbolic arctangent (inverse hyperbolic tangent) of a value
     * @param A Input value in the range (-1, 1)
     * @return The hyperbolic arctangent of the input value
     * @note The domain of ATanh is (-1, 1). Values outside this range will result in NaN or infinity.
     */
    ABYTEK_FORCE_INLINE F32 ATanh(F32 A) noexcept
    {
        return std::atanh(A);
    }
    
    /**
     * @brief Computes the hyperbolic arctangent (inverse hyperbolic tangent) of a value
     * @param A Input value in the range (-1, 1)
     * @return The hyperbolic arctangent of the input value
     * @note The domain of ATanh is (-1, 1). Values outside this range will result in NaN or infinity.
     */
    ABYTEK_FORCE_INLINE F64 ATanh(F64 A) noexcept
    {
        return std::atanh(A);
    }
}