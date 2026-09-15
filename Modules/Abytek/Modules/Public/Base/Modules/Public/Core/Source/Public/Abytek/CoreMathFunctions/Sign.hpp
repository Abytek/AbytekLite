#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Returns the sign of an 8-bit unsigned integer
     * 
     * Returns 0 if the value is 0, otherwise returns 1.
     * 
     * @param A Input unsigned 8-bit integer
     * @return U8 Sign value (0 or 1)
     */
    ABYTEK_FORCE_INLINE U8 Sign(U8 A) noexcept
    {
        if(A == 0)
        {
            return 0;
        }
        return 1;
    }
    
    /**
     * @brief Returns the sign of a 16-bit unsigned integer
     * 
     * Returns 0 if the value is 0, otherwise returns 1.
     * 
     * @param A Input unsigned 16-bit integer
     * @return U16 Sign value (0 or 1)
     */
    ABYTEK_FORCE_INLINE U16 Sign(U16 A) noexcept
    {
        if(A == 0)
        {
            return 0;
        }
        return 1;
    }
    
    /**
     * @brief Returns the sign of a 32-bit unsigned integer
     * 
     * Returns 0 if the value is 0, otherwise returns 1.
     * 
     * @param A Input unsigned 32-bit integer
     * @return U32 Sign value (0 or 1)
     */
    ABYTEK_FORCE_INLINE U32 Sign(U32 A) noexcept
    {
        if(A == 0)
        {
            return 0;
        }
        return 1;
    }
    
    /**
     * @brief Returns the sign of a 64-bit unsigned integer
     * 
     * Returns 0 if the value is 0, otherwise returns 1.
     * 
     * @param A Input unsigned 64-bit integer
     * @return U64 Sign value (0 or 1)
     */
    ABYTEK_FORCE_INLINE U64 Sign(U64 A) noexcept
    {
        if(A == 0)
        {
            return 0;
        }
        return 1;
    }
    
    /**
     * @brief Returns the sign of an 8-bit signed integer
     * 
     * Returns 1 if positive, -1 if negative, 0 if zero.
     * 
     * @param A Input signed 8-bit integer
     * @return I8 Sign value (-1, 0, or 1)
     */
    ABYTEK_FORCE_INLINE I8 Sign(I8 A) noexcept
    {
        if(A > 0)
        {
            return 1;
        }
        if(A < 0)
        {
            return -1;
        }
        return 0;
    }
    
    /**
     * @brief Returns the sign of a 16-bit signed integer
     * 
     * Returns 1 if positive, -1 if negative, 0 if zero.
     * 
     * @param A Input signed 16-bit integer
     * @return I16 Sign value (-1, 0, or 1)
     */
    ABYTEK_FORCE_INLINE I16 Sign(I16 A) noexcept
    {
        if(A > 0)
        {
            return 1;
        }
        if(A < 0)
        {
            return -1;
        }
        return 0;
    }
    
    /**
     * @brief Returns the sign of a 32-bit signed integer
     * 
     * Returns 1 if positive, -1 if negative, 0 if zero.
     * 
     * @param A Input signed 32-bit integer
     * @return I32 Sign value (-1, 0, or 1)
     */
    ABYTEK_FORCE_INLINE I32 Sign(I32 A) noexcept
    {
        if(A > 0)
        {
            return 1;
        }
        if(A < 0)
        {
            return -1;
        }
        return 0;
    }
    
    /**
     * @brief Returns the sign of a 64-bit signed integer
     * 
     * Returns 1 if positive, -1 if negative, 0 if zero.
     * 
     * @param A Input signed 64-bit integer
     * @return I64 Sign value (-1, 0, or 1)
     */
    ABYTEK_FORCE_INLINE I64 Sign(I64 A) noexcept
    {
        if(A > 0)
        {
            return 1;
        }
        if(A < 0)
        {
            return -1;
        }
        return 0;
    }
    
    /**
     * @brief Returns the sign of a float
     * 
     * Returns 1.0f if positive, -1.0f if negative, 0.0f if zero.
     * 
     * @param A Input floating-point value
     * @return F32 Sign value (-1.0f, 0.0f, or 1.0f)
     */
    ABYTEK_FORCE_INLINE F32 Sign(F32 A) noexcept
    {
        if(A > 0.0f)
        {
            return 1.0f;
        }
        if(A < 0.0f)
        {
            return -1.0f;
        }
        return 0.0f;
    }
    
    /**
     * @brief Returns the sign of a double
     * 
     * Returns 1.0 if positive, -1.0 if negative, 0.0 if zero.
     * 
     * @param A Input double-precision value
     * @return F64 Sign value (-1.0, 0.0, or 1.0)
     */
    ABYTEK_FORCE_INLINE F64 Sign(F64 A) noexcept
    {
        if(A > 0.0)
        {
            return 1.0;
        }
        if(A < 0.0)
        {
            return -1.0;
        }
        return 0.0;
    }
}