#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the square of an unsigned 8-bit integer.
     * @param A The value to square.
     * @return The square of the input value.
     */
    ABYTEK_FORCE_INLINE U8 Square(U8 A) noexcept
    {
        return A * A;
    }
    /**
     * @brief Calculates the square of an unsigned 16-bit integer.
     * @param A The value to square.
     * @return The square of the input value.
     */
    ABYTEK_FORCE_INLINE U16 Square(U16 A) noexcept
    {
        return A * A;
    }
    /**
     * @brief Calculates the square of an unsigned 32-bit integer.
     * @param A The value to square.
     * @return The square of the input value.
     */
    ABYTEK_FORCE_INLINE U32 Square(U32 A) noexcept
    {
        return A * A;
    }
    /**
     * @brief Calculates the square of an unsigned 64-bit integer.
     * @param A The value to square.
     * @return The square of the input value.
     */
    ABYTEK_FORCE_INLINE U64 Square(U64 A) noexcept
    {
        return A * A;
    }
    
    /**
     * @brief Calculates the square of a signed 8-bit integer.
     * @param A The value to square.
     * @return The square of the input value.
     */
    ABYTEK_FORCE_INLINE I8 Square(I8 A) noexcept
    {
        return A * A;
    }
    /**
     * @brief Calculates the square of a signed 16-bit integer.
     * @param A The value to square.
     * @return The square of the input value.
     */
    ABYTEK_FORCE_INLINE I16 Square(I16 A) noexcept
    {
        return A * A;
    }
    /**
     * @brief Calculates the square of a signed 32-bit integer.
     * @param A The value to square.
     * @return The square of the input value.
     */
    ABYTEK_FORCE_INLINE I32 Square(I32 A) noexcept
    {
        return A * A;
    }
    /**
     * @brief Calculates the square of a signed 64-bit integer.
     * @param A The value to square.
     * @return The square of the input value.
     */
    ABYTEK_FORCE_INLINE I64 Square(I64 A) noexcept
    {
        return A * A;
    }
    
    /**
     * @brief Calculates the square of a 32-bit floating point value.
     * @param A The value to square.
     * @return The square of the input value.
     */
    ABYTEK_FORCE_INLINE F32 Square(F32 A) noexcept
    {
        return A * A;
    }
    /**
     * @brief Calculates the square of a 64-bit floating point value.
     * @param A The value to square.
     * @return The square of the input value.
     */
    ABYTEK_FORCE_INLINE F64 Square(F64 A) noexcept
    {
        return A * A;
    }
}