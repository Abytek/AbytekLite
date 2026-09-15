#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Rounds a float to the nearest integer value (as an I32)
     * 
     * Rounds the input floating-point value to the nearest integer.
     * If the fractional part is >= 0.5, rounds up, otherwise rounds down.
     * 
     * @param A The floating-point value to round
     * @return I32 The rounded value as a 32-bit integer
     */
    ABYTEK_FORCE_INLINE I32 RoundToInt(F32 A) noexcept
    {
        I32 Result = static_cast<I32>(A);

        if((A - static_cast<F32>(Result)) >= 0.5f)
        {
            return Result + 1;    
        }
        return Result;
    }
    
    /**
     * @brief Rounds a double to the nearest integer value (as an I64)
     * 
     * Rounds the input double-precision value to the nearest integer.
     * If the fractional part is >= 0.5, rounds up, otherwise rounds down.
     * 
     * @param A The double-precision value to round
     * @return I64 The rounded value as a 64-bit integer
     */
    ABYTEK_FORCE_INLINE I64 RoundToInt(F64 A) noexcept
    {
        I64 Result = static_cast<I64>(A);

        if((A - static_cast<F64>(Result)) >= 0.5)
        {
            return Result + 1;    
        }
        return Result;
    }
}