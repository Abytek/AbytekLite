#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * Computes the power function for 32-bit floating point values.
     * Returns X raised to the power of Y.
     * 
     * @param X Base value
     * @param Y Exponent value
     * @return X raised to the power of Y
     */
    ABYTEK_FORCE_INLINE F32 Pow(F32 X, F32 Y) noexcept
    {
        return std::pow(X, Y);
    }
    
    /**
     * Computes the power function for 64-bit floating point values.
     * Returns X raised to the power of Y.
     * 
     * @param X Base value
     * @param Y Exponent value
     * @return X raised to the power of Y
     */
    ABYTEK_FORCE_INLINE F64 Pow(F64 X, F64 Y) noexcept
    {
        return std::pow(X, Y);
    }
}