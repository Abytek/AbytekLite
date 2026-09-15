#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * Returns the smaller of two unsigned 8-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The smaller value between A and B
     */
    ABYTEK_FORCE_INLINE U8 Min(U8 A, U8 B) noexcept
    {
        return (A < B) ? A : B;
    }
    
    /**
     * Returns the smaller of two unsigned 16-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The smaller value between A and B
     */
    ABYTEK_FORCE_INLINE U16 Min(U16 A, U16 B) noexcept
    {
        return (A < B) ? A : B;
    }
    
    /**
     * Returns the smaller of two unsigned 32-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The smaller value between A and B
     */
    ABYTEK_FORCE_INLINE U32 Min(U32 A, U32 B) noexcept
    {
        return (A < B) ? A : B;
    }
    
    /**
     * Returns the smaller of two unsigned 64-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The smaller value between A and B
     */
    ABYTEK_FORCE_INLINE U64 Min(U64 A, U64 B) noexcept
    {
        return (A < B) ? A : B;
    }
    
    /**
     * Returns the smaller of two signed 8-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The smaller value between A and B
     */
    ABYTEK_FORCE_INLINE I8 Min(I8 A, I8 B) noexcept
    {
        return (A < B) ? A : B;
    }
    
    /**
     * Returns the smaller of two signed 16-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The smaller value between A and B
     */
    ABYTEK_FORCE_INLINE I16 Min(I16 A, I16 B) noexcept
    {
        return (A < B) ? A : B;
    }
    
    /**
     * Returns the smaller of two signed 32-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The smaller value between A and B
     */
    ABYTEK_FORCE_INLINE I32 Min(I32 A, I32 B) noexcept
    {
        return (A < B) ? A : B;
    }
    
    /**
     * Returns the smaller of two signed 64-bit integer values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The smaller value between A and B
     */
    ABYTEK_FORCE_INLINE I64 Min(I64 A, I64 B) noexcept
    {
        return (A < B) ? A : B;
    }
        
    /**
     * Returns the smaller of two 32-bit floating point values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The smaller value between A and B
     */
    ABYTEK_FORCE_INLINE F32 Min(F32 A, F32 B) noexcept
    {
        return (A < B) ? A : B;
    }
    
    /**
     * Returns the smaller of two 64-bit floating point values.
     * 
     * @param A First value to compare
     * @param B Second value to compare
     * @return The smaller value between A and B
     */
    ABYTEK_FORCE_INLINE F64 Min(F64 A, F64 B) noexcept
    {
        return (A < B) ? A : B;
    }
}