#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Computes the hyperbolic cosine of a value
     * @param A Input value
     * @return The hyperbolic cosine of the input value
     * @note The hyperbolic cosine is always >= 1
     */
    ABYTEK_FORCE_INLINE F32 Cosh(F32 A) noexcept
    {
        return std::cosh(A);
    }
    
    /**
     * @brief Computes the hyperbolic cosine of a value
     * @param A Input value
     * @return The hyperbolic cosine of the input value
     * @note The hyperbolic cosine is always >= 1
     */
    ABYTEK_FORCE_INLINE F64 Cosh(F64 A) noexcept
    {
        return std::cosh(A);
    }
}