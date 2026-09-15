#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Functions/IdentityTemplate.hpp"


namespace Abytek
{
    /**
     * Template specialization for creating a 2x2 identity matrix with float values.
     * 
     * @return A 2x2 identity matrix where diagonal elements are 1.0f and all others are 0.0f
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Identity<F_Matrix2x2_F32>() noexcept
    {
        return {
            F_Vector2_F32::Right(),
            F_Vector2_F32::Up()
        };
    }
    
    /**
     * Template specialization for creating a 2x2 identity matrix with signed integer values.
     * 
     * @return A 2x2 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix2x2_I32 Identity<F_Matrix2x2_I32>() noexcept
    {
        return {
            F_Vector2_I32::Right(),
            F_Vector2_I32::Up()
        };
    }
    
    /**
     * Template specialization for creating a 2x2 identity matrix with unsigned integer values.
     * 
     * @return A 2x2 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix2x2_U32 Identity<F_Matrix2x2_U32>() noexcept
    {
        return {
            F_Vector2_U32::Right(),
            F_Vector2_U32::Up()
        };
    }
    
    /**
     * Template specialization for creating a 2x2 identity matrix with unsigned integer values.
     * 
     * @return A 2x2 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix2x2_U16 Identity<F_Matrix2x2_U16>() noexcept
    {
        return {
            F_Vector2_U16::Right(),
            F_Vector2_U16::Up()
        };
    }
    
    /**
     * Template specialization for creating a 2x2 identity matrix with unsigned integer values.
     * 
     * @return A 2x2 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix2x2_U8 Identity<F_Matrix2x2_U8>() noexcept
    {
        return {
            F_Vector2_U8::Right(),
            F_Vector2_U8::Up()
        };
    }
    
    /**
     * Template specialization for creating a 2x2 identity matrix with unsigned integer values.
     * 
     * @return A 2x2 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix2x2_I64 Identity<F_Matrix2x2_I64>() noexcept
    {
        return {
            F_Vector2_I64::Right(),
            F_Vector2_I64::Up()
        };
    }
    
    /**
     * Template specialization for creating a 2x2 identity matrix with unsigned integer values.
     * 
     * @return A 2x2 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix2x2_U64 Identity<F_Matrix2x2_U64>() noexcept
    {
        return {
            F_Vector2_U64::Right(),
            F_Vector2_U64::Up()
        };
    }
    
    /**
     * Template specialization for creating a 2x2 identity matrix with unsigned integer values.
     * 
     * @return A 2x2 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix2x2_I8 Identity<F_Matrix2x2_I8>() noexcept
    {
        return {
            F_Vector2_I8::Right(),
            F_Vector2_I8::Up()
        };
    }
    
    /**
     * Template specialization for creating a 2x2 identity matrix with unsigned integer values.
     * 
     * @return A 2x2 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix2x2_I16 Identity<F_Matrix2x2_I16>() noexcept
    {
        return {
            F_Vector2_I16::Right(),
            F_Vector2_I16::Up()
        };
    }
    
    /**
     * Template specialization for creating a 3x3 identity matrix with float values.
     * 
     * @return A 3x3 identity matrix where diagonal elements are 1.0f and all others are 0.0f
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Identity<F_Matrix3x3_F32>() noexcept
    {
        return {
            F_Vector3_F32::Right(),
            F_Vector3_F32::Up(),
            F_Vector3_F32::Forward()
        };
    }
    
    /**
     * Template specialization for creating a 3x3 identity matrix with signed integer values.
     * 
     * @return A 3x3 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix3x3_I32 Identity<F_Matrix3x3_I32>() noexcept
    {
        return {
            F_Vector3_I32::Right(),
            F_Vector3_I32::Up(),
            F_Vector3_I32::Forward()
        };
    }
    
    /**
     * Template specialization for creating a 3x3 identity matrix with unsigned integer values.
     * 
     * @return A 3x3 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix3x3_U32 Identity<F_Matrix3x3_U32>() noexcept
    {
        return {
            F_Vector3_U32::Right(),
            F_Vector3_U32::Up(),
            F_Vector3_U32::Forward()
        };
    }
    
    /**
     * Template specialization for creating a 3x3 identity matrix with unsigned integer values.
     * 
     * @return A 3x3 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix3x3_U16 Identity<F_Matrix3x3_U16>() noexcept
    {
        return {
            F_Vector3_U16::Right(),
            F_Vector3_U16::Up(),
            F_Vector3_U16::Forward()
        };
    }
    
    /**
     * Template specialization for creating a 3x3 identity matrix with unsigned integer values.
     * 
     * @return A 3x3 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix3x3_U8 Identity<F_Matrix3x3_U8>() noexcept
    {
        return {
            F_Vector3_U8::Right(),
            F_Vector3_U8::Up(),
            F_Vector3_U8::Forward()
        };
    }
    
    /**
     * Template specialization for creating a 3x3 identity matrix with unsigned integer values.
     * 
     * @return A 3x3 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix3x3_I64 Identity<F_Matrix3x3_I64>() noexcept
    {
        return {
            F_Vector3_I64::Right(),
            F_Vector3_I64::Up(),
            F_Vector3_I64::Forward()
        };
    }
    
    /**
     * Template specialization for creating a 3x3 identity matrix with unsigned integer values.
     * 
     * @return A 3x3 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix3x3_U64 Identity<F_Matrix3x3_U64>() noexcept
    {
        return {
            F_Vector3_U64::Right(),
            F_Vector3_U64::Up(),
            F_Vector3_U64::Forward()
        };
    }
    
    /**
     * Template specialization for creating a 3x3 identity matrix with unsigned integer values.
     * 
     * @return A 3x3 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix3x3_I8 Identity<F_Matrix3x3_I8>() noexcept
    {
        return {
            F_Vector3_I8::Right(),
            F_Vector3_I8::Up(),
            F_Vector3_I8::Forward()
        };
    }
    
    /**
     * Template specialization for creating a 3x3 identity matrix with unsigned integer values.
     * 
     * @return A 3x3 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix3x3_I16 Identity<F_Matrix3x3_I16>() noexcept
    {
        return {
            F_Vector3_I16::Right(),
            F_Vector3_I16::Up(),
            F_Vector3_I16::Forward()
        };
    }
    
    /**
     * Template specialization for creating a 4x4 identity matrix with float values.
     * 
     * @return A 4x4 identity matrix where diagonal elements are 1.0f and all others are 0.0f
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Identity<F_Matrix4x4_F32>() noexcept
    {
        return {
            F_Vector4_F32::Right(),
            F_Vector4_F32::Up(),
            F_Vector4_F32::Forward(),
            F_Vector4_F32::Future()
        };
    }
    
    /**
     * Template specialization for creating a 4x4 identity matrix with signed integer values.
     * 
     * @return A 4x4 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix4x4_I32 Identity<F_Matrix4x4_I32>() noexcept
    {
        return {
            F_Vector4_I32::Right(),
            F_Vector4_I32::Up(),
            F_Vector4_I32::Forward(),
            F_Vector4_I32::Future()
        };
    }
    
    /**
     * Template specialization for creating a 4x4 identity matrix with unsigned integer values.
     * 
     * @return A 4x4 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix4x4_U32 Identity<F_Matrix4x4_U32>() noexcept
    {
        return {
            F_Vector4_U32::Right(),
            F_Vector4_U32::Up(),
            F_Vector4_U32::Forward(),
            F_Vector4_U32::Future()
        };
    }
    
    /**
     * Template specialization for creating a 4x4 identity matrix with unsigned integer values.
     * 
     * @return A 4x4 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix4x4_U16 Identity<F_Matrix4x4_U16>() noexcept
    {
        return {
            F_Vector4_U16::Right(),
            F_Vector4_U16::Up(),
            F_Vector4_U16::Forward(),
            F_Vector4_U16::Future()
        };
    }
    
    /**
     * Template specialization for creating a 4x4 identity matrix with unsigned integer values.
     * 
     * @return A 4x4 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix4x4_U8 Identity<F_Matrix4x4_U8>() noexcept
    {
        return {
            F_Vector4_U8::Right(),
            F_Vector4_U8::Up(),
            F_Vector4_U8::Forward(),
            F_Vector4_U8::Future()
        };
    }
    
    /**
     * Template specialization for creating a 4x4 identity matrix with unsigned integer values.
     * 
     * @return A 4x4 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix4x4_I64 Identity<F_Matrix4x4_I64>() noexcept
    {
        return {
            F_Vector4_I64::Right(),
            F_Vector4_I64::Up(),
            F_Vector4_I64::Forward(),
            F_Vector4_I64::Future()
        };
    }
    
    /**
     * Template specialization for creating a 4x4 identity matrix with unsigned integer values.
     * 
     * @return A 4x4 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix4x4_U64 Identity<F_Matrix4x4_U64>() noexcept
    {
        return {
            F_Vector4_U64::Right(),
            F_Vector4_U64::Up(),
            F_Vector4_U64::Forward(),
            F_Vector4_U64::Future()
        };
    }
    
    /**
     * Template specialization for creating a 4x4 identity matrix with unsigned integer values.
     * 
     * @return A 4x4 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix4x4_I8 Identity<F_Matrix4x4_I8>() noexcept
    {
        return {
            F_Vector4_I8::Right(),
            F_Vector4_I8::Up(),
            F_Vector4_I8::Forward(),
            F_Vector4_I8::Future()
        };
    }
    
    /**
     * Template specialization for creating a 4x4 identity matrix with unsigned integer values.
     * 
     * @return A 4x4 identity matrix where diagonal elements are 1 and all others are 0
     */
    template<>
    ABYTEK_FORCE_INLINE F_Matrix4x4_I16 Identity<F_Matrix4x4_I16>() noexcept
    {
        return {
            F_Vector4_I16::Right(),
            F_Vector4_I16::Up(),
            F_Vector4_I16::Forward(),
            F_Vector4_I16::Future()
        };
    }
}