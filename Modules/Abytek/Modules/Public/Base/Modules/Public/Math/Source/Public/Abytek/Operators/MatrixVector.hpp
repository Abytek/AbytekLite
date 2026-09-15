/**
 * @file MatrixVector.hpp
 * @brief Defines matrix-vector multiplication operations for various numeric types.
 * 
 * This file contains operator overloads for matrix-vector multiplications:
 * - Multiplication of matrices and vectors (matrix * vector)
 * 
 * Operations are defined for 2x2, 3x3, and 4x4 matrices with corresponding vector sizes,
 * and for F32 (floating point), I32 (signed integer), and U32 (unsigned integer) types.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data.hpp"
#include "Abytek/Functions/Dot.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek::Internal::MathOperators
{
    /**
     * @brief Multiplication operator for a 2x2 F32 matrix and a 2D F32 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32 operator * (Abytek::PA_Matrix2x2_F32 A, Abytek::PA_Vector2_F32 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
        );
    }
    /**
     * @brief Multiplication operator for a 3x3 F32 matrix and a 3D F32 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32 operator * (Abytek::PA_Matrix3x3_F32 A, Abytek::PA_Vector3_F32 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
        );
    }
    /**
     * @brief Multiplication operator for a 4x4 F32 matrix and a 4D F32 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32 operator * (Abytek::PA_Matrix4x4_F32 A, Abytek::PA_Vector4_F32 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
            + A.D * B.W
        );
    }

    /**
     * @brief Multiplication operator for a 2x2 I32 matrix and a 2D I32 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I32 operator * (Abytek::PA_Matrix2x2_I32 A, Abytek::PA_Vector2_I32 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
        );
    }
    /**
     * @brief Multiplication operator for a 3x3 I32 matrix and a 3D I32 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I32 operator * (Abytek::PA_Matrix3x3_I32 A, Abytek::PA_Vector3_I32 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
        );
    }
    /**
     * @brief Multiplication operator for a 4x4 I32 matrix and a 4D I32 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I32 operator * (Abytek::PA_Matrix4x4_I32 A, Abytek::PA_Vector4_I32 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
            + A.D * B.W
        );
    }

    /**
     * @brief Multiplication operator for a 2x2 I16 matrix and a 2D I16 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I16 operator * (Abytek::PA_Matrix2x2_I16 A, Abytek::PA_Vector2_I16 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
        );
    }
    /**
     * @brief Multiplication operator for a 3x3 I16 matrix and a 3D I16 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I16 operator * (Abytek::PA_Matrix3x3_I16 A, Abytek::PA_Vector3_I16 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
        );
    }
    /**
     * @brief Multiplication operator for a 4x4 I16 matrix and a 4D I16 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I16 operator * (Abytek::PA_Matrix4x4_I16 A, Abytek::PA_Vector4_I16 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
            + A.D * B.W
        );
    }

    /**
     * @brief Multiplication operator for a 2x2 I8 matrix and a 2D I8 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8 operator * (Abytek::PA_Matrix2x2_I8 A, Abytek::PA_Vector2_I8 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
        );
    }
    /**
     * @brief Multiplication operator for a 3x3 I8 matrix and a 3D I8 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8 operator * (Abytek::PA_Matrix3x3_I8 A, Abytek::PA_Vector3_I8 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
        );
    }
    /**
     * @brief Multiplication operator for a 4x4 I8 matrix and a 4D I8 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8 operator * (Abytek::PA_Matrix4x4_I8 A, Abytek::PA_Vector4_I8 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
            + A.D * B.W
        );
    }

    /**
     * @brief Multiplication operator for a 2x2 U32 matrix and a 2D U32 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U32 operator * (Abytek::PA_Matrix2x2_U32 A, Abytek::PA_Vector2_U32 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
        );
    }
    /**
     * @brief Multiplication operator for a 3x3 U32 matrix and a 3D U32 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U32 operator * (Abytek::PA_Matrix3x3_U32 A, Abytek::PA_Vector3_U32 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
        );
    }
    /**
     * @brief Multiplication operator for a 4x4 U32 matrix and a 4D U32 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U32 operator * (Abytek::PA_Matrix4x4_U32 A, Abytek::PA_Vector4_U32 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
            + A.D * B.W
        );
    }

    /**
     * @brief Multiplication operator for a 2x2 U16 matrix and a 2D U16 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U16 operator * (Abytek::PA_Matrix2x2_U16 A, Abytek::PA_Vector2_U16 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
        );
    }
    /**
     * @brief Multiplication operator for a 3x3 U16 matrix and a 3D U16 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U16 operator * (Abytek::PA_Matrix3x3_U16 A, Abytek::PA_Vector3_U16 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
        );
    }
    /**
     * @brief Multiplication operator for a 4x4 U16 matrix and a 4D U16 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U16 operator * (Abytek::PA_Matrix4x4_U16 A, Abytek::PA_Vector4_U16 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
            + A.D * B.W
        );
    }

    /**
     * @brief Multiplication operator for a 2x2 U8 matrix and a 2D U8 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8 operator * (Abytek::PA_Matrix2x2_U8 A, Abytek::PA_Vector2_U8 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
        );
    }
    /**
     * @brief Multiplication operator for a 3x3 U8 matrix and a 3D U8 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8 operator * (Abytek::PA_Matrix3x3_U8 A, Abytek::PA_Vector3_U8 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
        );
    }
    /**
     * @brief Multiplication operator for a 4x4 U8 matrix and a 4D U8 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8 operator * (Abytek::PA_Matrix4x4_U8 A, Abytek::PA_Vector4_U8 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
            + A.D * B.W
        );
    }

    /**
     * @brief Multiplication operator for a 2x2 I64 matrix and a 2D I64 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I64 operator * (Abytek::PA_Matrix2x2_I64 A, Abytek::PA_Vector2_I64 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
        );
    }
    /**
     * @brief Multiplication operator for a 3x3 I64 matrix and a 3D I64 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I64 operator * (Abytek::PA_Matrix3x3_I64 A, Abytek::PA_Vector3_I64 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
        );
    }
    /**
     * @brief Multiplication operator for a 4x4 I64 matrix and a 4D I64 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I64 operator * (Abytek::PA_Matrix4x4_I64 A, Abytek::PA_Vector4_I64 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
            + A.D * B.W
        );
    }

    /**
     * @brief Multiplication operator for a 2x2 U64 matrix and a 2D U64 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U64 operator * (Abytek::PA_Matrix2x2_U64 A, Abytek::PA_Vector2_U64 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
        );
    }
    /**
     * @brief Multiplication operator for a 3x3 U64 matrix and a 3D U64 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U64 operator * (Abytek::PA_Matrix3x3_U64 A, Abytek::PA_Vector3_U64 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
        );
    }
    /**
     * @brief Multiplication operator for a 4x4 U64 matrix and a 4D U64 vector.
     * @param A The matrix.
     * @param B The vector.
     * @return The transformed vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U64 operator * (Abytek::PA_Matrix4x4_U64 A, Abytek::PA_Vector4_U64 B) noexcept
    {
        return (
            A.A * B.X
            + A.B * B.Y
            + A.C * B.Z
            + A.D * B.W
        );
    }
}