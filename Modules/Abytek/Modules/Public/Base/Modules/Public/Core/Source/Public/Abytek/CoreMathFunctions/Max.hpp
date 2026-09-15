#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * Returns the greater of two unsigned 8-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The greater value between A and B
     */
    ABYTEK_FORCE_INLINE U8 Max(U8 A, U8 B) noexcept
    {
        return (A > B) ? A : B;
    }
    
    /**
     * Returns the greater of two unsigned 16-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The greater value between A and B
     */
    ABYTEK_FORCE_INLINE U16 Max(U16 A, U16 B) noexcept
    {
        return (A > B) ? A : B;
    }
    
    /**
     * Returns the greater of two unsigned 32-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The greater value between A and B
     */
    ABYTEK_FORCE_INLINE U32 Max(U32 A, U32 B) noexcept
    {
        return (A > B) ? A : B;
    }
    
    /**
     * Returns the greater of two unsigned 64-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The greater value between A and B
     */
    ABYTEK_FORCE_INLINE U64 Max(U64 A, U64 B) noexcept
    {
        return (A > B) ? A : B;
    }
    
    /**
     * Returns the greater of two signed 8-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The greater value between A and B
     */
    ABYTEK_FORCE_INLINE I8 Max(I8 A, I8 B) noexcept
    {
        return (A > B) ? A : B;
    }
    
    /**
     * Returns the greater of two signed 16-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The greater value between A and B
     */
    ABYTEK_FORCE_INLINE I16 Max(I16 A, I16 B) noexcept
    {
        return (A > B) ? A : B;
    }
    
    /**
     * Returns the greater of two signed 32-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The greater value between A and B
     */
    ABYTEK_FORCE_INLINE I32 Max(I32 A, I32 B) noexcept
    {
        return (A > B) ? A : B;
    }
    
    /**
     * Returns the greater of two signed 64-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The greater value between A and B
     */
    ABYTEK_FORCE_INLINE I64 Max(I64 A, I64 B) noexcept
    {
        return (A > B) ? A : B;
    }
    
    /**
     * Returns the greater of two 32-bit floating point values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The greater value between A and B
     */
    ABYTEK_FORCE_INLINE F32 Max(F32 A, F32 B) noexcept
    {
        return (A > B) ? A : B;
    }
    
    /**
     * Returns the greater of two 64-bit floating point values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The greater value between A and B
     */
    ABYTEK_FORCE_INLINE F64 Max(F64 A, F64 B) noexcept
    {
        return (A > B) ? A : B;
    }
}