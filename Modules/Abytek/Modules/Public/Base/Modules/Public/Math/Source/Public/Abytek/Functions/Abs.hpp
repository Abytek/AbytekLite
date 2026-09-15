#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"

/**
 * @file Abs.hpp
 * @brief Provides absolute value functions for various numeric types, vectors, matrices, and quaternions.
 * 
 * This file contains overloaded Abs functions that return the absolute value of different data types.
 * For unsigned types, the value is returned unchanged.
 * For signed types, the function returns the magnitude of the value without its sign.
 * For complex types like vectors and matrices, the function applies Abs to each component.
 */
namespace Abytek
{
    /**
     * @brief Returns the component-wise absolute value of a 2D float vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Abs(PA_Vector2_F32 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2D signed integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_I32 Abs(PA_Vector2_I32 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_U32 Abs(PA_Vector2_U32 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_U16 Abs(PA_Vector2_U16 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_U8 Abs(PA_Vector2_U8 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_I64 Abs(PA_Vector2_I64 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_U64 Abs(PA_Vector2_U64 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_I8 Abs(PA_Vector2_I8 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_I16 Abs(PA_Vector2_I16 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y)
        };
    }
    
    /**
     * @brief Returns the component-wise absolute value of a 3D float vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Abs(PA_Vector3_F32 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3D signed integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_I32 Abs(PA_Vector3_I32 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_U32 Abs(PA_Vector3_U32 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_U16 Abs(PA_Vector3_U16 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_U8 Abs(PA_Vector3_U8 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_I64 Abs(PA_Vector3_I64 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_U64 Abs(PA_Vector3_U64 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_I8 Abs(PA_Vector3_I8 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_I16 Abs(PA_Vector3_I16 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z)
        };
    }
    
    /**
     * @brief Returns the component-wise absolute value of a 4D float vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Abs(PA_Vector4_F32 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z),
            Abs(A.W)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4D signed integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_I32 Abs(PA_Vector4_I32 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z),
            Abs(A.W)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_U32 Abs(PA_Vector4_U32 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z),
            Abs(A.W)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_U16 Abs(PA_Vector4_U16 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z),
            Abs(A.W)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_U8 Abs(PA_Vector4_U8 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z),
            Abs(A.W)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_I64 Abs(PA_Vector4_I64 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z),
            Abs(A.W)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_U64 Abs(PA_Vector4_U64 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z),
            Abs(A.W)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_I8 Abs(PA_Vector4_I8 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z),
            Abs(A.W)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4D unsigned integer vector.
     * @param A The input vector.
     * @return A vector with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_I16 Abs(PA_Vector4_I16 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z),
            Abs(A.W)
        };
    }
    
    /**
     * @brief Returns the component-wise absolute value of a 2x2 float matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Abs(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2x2 signed integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I32 Abs(PA_Matrix2x2_I32 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2x2 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U32 Abs(PA_Matrix2x2_U32 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2x2 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U16 Abs(PA_Matrix2x2_U16 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2x2 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U8 Abs(PA_Matrix2x2_U8 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2x2 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I64 Abs(PA_Matrix2x2_I64 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2x2 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U64 Abs(PA_Matrix2x2_U64 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2x2 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I8 Abs(PA_Matrix2x2_I8 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 2x2 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I16 Abs(PA_Matrix2x2_I16 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B)
        };
    }
    
    /**
     * @brief Returns the component-wise absolute value of a 3x3 float matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Abs(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3x3 signed integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I32 Abs(PA_Matrix3x3_I32 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3x3 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U32 Abs(PA_Matrix3x3_U32 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3x3 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U16 Abs(PA_Matrix3x3_U16 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3x3 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U8 Abs(PA_Matrix3x3_U8 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3x3 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I64 Abs(PA_Matrix3x3_I64 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3x3 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U64 Abs(PA_Matrix3x3_U64 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3x3 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I8 Abs(PA_Matrix3x3_I8 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 3x3 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I16 Abs(PA_Matrix3x3_I16 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C)
        };
    }
    
    /**
     * @brief Returns the component-wise absolute value of a 4x4 float matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Abs(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C),
            Abs(A.D)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4x4 signed integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I32 Abs(PA_Matrix4x4_I32 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C),
            Abs(A.D)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4x4 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U32 Abs(PA_Matrix4x4_U32 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C),
            Abs(A.D)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4x4 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U16 Abs(PA_Matrix4x4_U16 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C),
            Abs(A.D)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4x4 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U8 Abs(PA_Matrix4x4_U8 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C),
            Abs(A.D)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4x4 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I64 Abs(PA_Matrix4x4_I64 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C),
            Abs(A.D)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4x4 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U64 Abs(PA_Matrix4x4_U64 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C),
            Abs(A.D)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4x4 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I8 Abs(PA_Matrix4x4_I8 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C),
            Abs(A.D)
        };
    }
    /**
     * @brief Returns the component-wise absolute value of a 4x4 unsigned integer matrix.
     * @param A The input matrix.
     * @return A matrix with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I16 Abs(PA_Matrix4x4_I16 A) noexcept
    {
        return {
            Abs(A.A),
            Abs(A.B),
            Abs(A.C),
            Abs(A.D)
        };
    }
    
    /**
     * @brief Returns the component-wise absolute value of a float quaternion.
     * @param A The input quaternion.
     * @return A quaternion with the absolute value of each component.
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Abs(PA_Quaternion_F32 A) noexcept
    {
        return {
            Abs(A.X),
            Abs(A.Y),
            Abs(A.Z),
            Abs(A.W)
        };
    }
}