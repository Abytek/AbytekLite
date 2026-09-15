/**
 * @file Square.hpp
 * @brief Functions for calculating the square of a value (A²).
 * 
 * Provides optimized, inlined square functions for various numeric types and vectors.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the component-wise square of a 2D float vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Square(PA_Vector2_F32 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2D signed integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_I32 Square(PA_Vector2_I32 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_U32 Square(PA_Vector2_U32 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_U16 Square(PA_Vector2_U16 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_U8 Square(PA_Vector2_U8 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_I8 Square(PA_Vector2_I8 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_I16 Square(PA_Vector2_I16 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y)
        };
    }
    
    /**
     * @brief Calculates the component-wise square of a 3D float vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Square(PA_Vector3_F32 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3D signed integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_I32 Square(PA_Vector3_I32 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_U32 Square(PA_Vector3_U32 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_U16 Square(PA_Vector3_U16 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_U8 Square(PA_Vector3_U8 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_I8 Square(PA_Vector3_I8 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_I16 Square(PA_Vector3_I16 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z)
        };
    }
    
    /**
     * @brief Calculates the component-wise square of a 4D float vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Square(PA_Vector4_F32 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z),
            Square(A.W)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4D signed integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_I32 Square(PA_Vector4_I32 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z),
            Square(A.W)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_U32 Square(PA_Vector4_U32 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z),
            Square(A.W)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_U16 Square(PA_Vector4_U16 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z),
            Square(A.W)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_U8 Square(PA_Vector4_U8 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z),
            Square(A.W)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_I8 Square(PA_Vector4_I8 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z),
            Square(A.W)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4D unsigned integer vector.
     * @param A The vector to square.
     * @return A vector containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_I16 Square(PA_Vector4_I16 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z),
            Square(A.W)
        };
    }
    
    /**
     * @brief Calculates the component-wise square of a 2x2 float matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Square(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2x2 signed integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I32 Square(PA_Matrix2x2_I32 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2x2 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U32 Square(PA_Matrix2x2_U32 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2x2 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U16 Square(PA_Matrix2x2_U16 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2x2 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U8 Square(PA_Matrix2x2_U8 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2x2 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I8 Square(PA_Matrix2x2_I8 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 2x2 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I16 Square(PA_Matrix2x2_I16 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B)
        };
    }
    
    /**
     * @brief Calculates the component-wise square of a 3x3 float matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Square(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3x3 signed integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I32 Square(PA_Matrix3x3_I32 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3x3 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U32 Square(PA_Matrix3x3_U32 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3x3 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U16 Square(PA_Matrix3x3_U16 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3x3 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U8 Square(PA_Matrix3x3_U8 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3x3 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I8 Square(PA_Matrix3x3_I8 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 3x3 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I16 Square(PA_Matrix3x3_I16 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C)
        };
    }
    
    /**
     * @brief Calculates the component-wise square of a 4x4 float matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Square(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C),
            Square(A.D)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4x4 signed integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I32 Square(PA_Matrix4x4_I32 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C),
            Square(A.D)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4x4 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U32 Square(PA_Matrix4x4_U32 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C),
            Square(A.D)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4x4 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U16 Square(PA_Matrix4x4_U16 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C),
            Square(A.D)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4x4 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U8 Square(PA_Matrix4x4_U8 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C),
            Square(A.D)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4x4 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I8 Square(PA_Matrix4x4_I8 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C),
            Square(A.D)
        };
    }
    /**
     * @brief Calculates the component-wise square of a 4x4 unsigned integer matrix.
     * @param A The matrix to square.
     * @return A matrix containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I16 Square(PA_Matrix4x4_I16 A) noexcept
    {
        return {
            Square(A.A),
            Square(A.B),
            Square(A.C),
            Square(A.D)
        };
    }
    
    /**
     * @brief Calculates the component-wise square of a quaternion.
     * @param A The quaternion to square.
     * @return A quaternion containing the square of each component.
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Square(PA_Quaternion_F32 A) noexcept
    {
        return {
            Square(A.X),
            Square(A.Y),
            Square(A.Z),
            Square(A.W)
        };
    }
}