#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Returns the absolute value of an unsigned 8-bit integer.
     * @param A The input value.
     * @return The input value (unchanged since unsigned values are always positive).
     */
    ABYTEK_FORCE_INLINE U8 Abs(U8 A) noexcept
    {
        return A;
    }
    /**
     * @brief Returns the absolute value of an unsigned 16-bit integer.
     * @param A The input value.
     * @return The input value (unchanged since unsigned values are always positive).
     */
    ABYTEK_FORCE_INLINE U16 Abs(U16 A) noexcept
    {
        return A;
    }
    /**
     * @brief Returns the absolute value of an unsigned 32-bit integer.
     * @param A The input value.
     * @return The input value (unchanged since unsigned values are always positive).
     */
    ABYTEK_FORCE_INLINE U32 Abs(U32 A) noexcept
    {
        return A;
    }
    /**
     * @brief Returns the absolute value of an unsigned 64-bit integer.
     * @param A The input value.
     * @return The input value (unchanged since unsigned values are always positive).
     */
    ABYTEK_FORCE_INLINE U64 Abs(U64 A) noexcept
    {
        return A;
    }
    
    /**
     * @brief Returns the absolute value of a signed 8-bit integer.
     * @param A The input value.
     * @return The absolute value of the input.
     */
    ABYTEK_FORCE_INLINE I8 Abs(I8 A) noexcept
    {
        return std::abs(A);
    }
    /**
     * @brief Returns the absolute value of a signed 16-bit integer.
     * @param A The input value.
     * @return The absolute value of the input.
     */
    ABYTEK_FORCE_INLINE I16 Abs(I16 A) noexcept
    {
        return std::abs(A);
    }
    /**
     * @brief Returns the absolute value of a signed 32-bit integer.
     * @param A The input value.
     * @return The absolute value of the input.
     */
    ABYTEK_FORCE_INLINE I32 Abs(I32 A) noexcept
    {
        return std::abs(A);
    }
    /**
     * @brief Returns the absolute value of a signed 64-bit integer.
     * @param A The input value.
     * @return The absolute value of the input.
     */
    ABYTEK_FORCE_INLINE I64 Abs(I64 A) noexcept
    {
        return std::abs(A);
    }
    
    /**
     * @brief Returns the absolute value of a 32-bit floating point number.
     * @param A The input value.
     * @return The absolute value of the input.
     */
    ABYTEK_FORCE_INLINE F32 Abs(F32 A) noexcept
    {
        return std::abs(A);
    }
    /**
     * @brief Returns the absolute value of a 64-bit floating point number.
     * @param A The input value.
     * @return The absolute value of the input.
     */
    ABYTEK_FORCE_INLINE F64 Abs(F64 A) noexcept
    {
        return std::abs(A);
    }
}