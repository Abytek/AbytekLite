/**
 * @file QuaternionMatrix.hpp
 * @brief Defines operations between quaternions and matrices.
 * 
 * This file contains operator overloads for interactions between quaternions and matrices:
 * - Multiplication of quaternions and matrices (quaternion * matrix)
 * - Division of quaternions and matrices (quaternion / matrix) 
 * - Multiplication of matrices and quaternions (matrix * quaternion)
 * - Division of matrices and quaternions (matrix / quaternion)
 * - Compound assignment operations (*=, /=) for matrices with quaternions
 * 
 * Operations are defined for 2x2, 3x3, and 4x4 matrices with F32 (floating point) quaternions.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/MatrixIdentity.hpp"
#include "Abytek/Functions/MatrixInverse.hpp"
#include "Abytek/Functions/QuaternionInverse.hpp"
#include "Abytek/Operators/MatrixMatrix.hpp"
#include "Abytek/Operators/QuaternionVector.hpp"


namespace Abytek::Internal::MathOperators
{
    /**
     * @brief Multiplication operator for a quaternion and a 2x2 matrix.
     * @param A The quaternion.
     * @param B The matrix.
     * @return The transformed matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator * (Abytek::PA_Quaternion_F32 A, Abytek::PA_Matrix2x2_F32 B) noexcept
    {
        return {
            A * B.A,
            A * B.B,
        };
    }
    /**
     * @brief Multiplication operator for a quaternion and a 3x3 matrix.
     * @param A The quaternion.
     * @param B The matrix.
     * @return The transformed matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator * (Abytek::PA_Quaternion_F32 A, Abytek::PA_Matrix3x3_F32 B) noexcept
    {
        return {
            A * B.A,
            A * B.B,
            A * B.C
        };
    }
    /**
     * @brief Multiplication operator for a quaternion and a 4x4 matrix.
     * @param A The quaternion.
     * @param B The matrix.
     * @return The transformed matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator * (Abytek::PA_Quaternion_F32 A, Abytek::PA_Matrix4x4_F32 B) noexcept
    {
        return {
            A * B.A,
            A * B.B,
            A * B.C,
            A * B.D
        };
    }

    /**
     * @brief Division operator for a quaternion and a 2x2 matrix.
     * @param A The quaternion.
     * @param B The matrix.
     * @return The result of A * inverse(B).
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator / (Abytek::PA_Quaternion_F32 A, Abytek::PA_Matrix2x2_F32 B) noexcept
    {
        return A * Abytek::Inverse(B);
    }
    /**
     * @brief Division operator for a quaternion and a 3x3 matrix.
     * @param A The quaternion.
     * @param B The matrix.
     * @return The result of A * inverse(B).
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator / (Abytek::PA_Quaternion_F32 A, Abytek::PA_Matrix3x3_F32 B) noexcept
    {
        return A * Abytek::Inverse(B);
    }
    /**
     * @brief Division operator for a quaternion and a 4x4 matrix.
     * @param A The quaternion.
     * @param B The matrix.
     * @return The result of A * inverse(B).
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator / (Abytek::PA_Quaternion_F32 A, Abytek::PA_Matrix4x4_F32 B) noexcept
    {
        return A * Abytek::Inverse(B);
    }

    /**
     * @brief Multiplication operator for a 2x2 matrix and a quaternion.
     * @param A The matrix.
     * @param B The quaternion.
     * @return The transformed matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator * (Abytek::PA_Matrix2x2_F32 A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        return A * (B * Abytek::Identity<Abytek::F_Matrix2x2_F32>());
    }
    /**
     * @brief Compound multiplication operator for a 2x2 matrix and a quaternion.
     * @param A The matrix to be modified.
     * @param B The quaternion to multiply by.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32& operator *= (Abytek::F_Matrix2x2_F32& A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        A *= (B * Abytek::Identity<Abytek::F_Matrix2x2_F32>());
        return A;
    }
    /**
     * @brief Multiplication operator for a 3x3 matrix and a quaternion.
     * @param A The matrix.
     * @param B The quaternion.
     * @return The transformed matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator * (Abytek::PA_Matrix3x3_F32 A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        return A * (B * Abytek::Identity<Abytek::F_Matrix3x3_F32>());
    }
    /**
     * @brief Compound multiplication operator for a 3x3 matrix and a quaternion.
     * @param A The matrix to be modified.
     * @param B The quaternion to multiply by.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32& operator *= (Abytek::F_Matrix3x3_F32& A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        A *= (B * Abytek::Identity<Abytek::F_Matrix3x3_F32>());
        return A;
    }
    /**
     * @brief Multiplication operator for a 4x4 matrix and a quaternion.
     * @param A The matrix.
     * @param B The quaternion.
     * @return The transformed matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator * (Abytek::PA_Matrix4x4_F32 A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        return A * (B * Abytek::Identity<Abytek::F_Matrix4x4_F32>());
    }
    /**
     * @brief Compound multiplication operator for a 4x4 matrix and a quaternion.
     * @param A The matrix to be modified.
     * @param B The quaternion to multiply by.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32& operator *= (Abytek::F_Matrix4x4_F32& A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        A *= (B * Abytek::Identity<Abytek::F_Matrix4x4_F32>());
        return A;
    }

    /**
     * @brief Division operator for a 2x2 matrix and a quaternion.
     * @param A The matrix.
     * @param B The quaternion.
     * @return The result of A * inverse(B).
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator / (Abytek::PA_Matrix2x2_F32 A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        return A * Abytek::Inverse(B);
    }
    /**
     * @brief Compound division operator for a 2x2 matrix and a quaternion.
     * @param A The matrix to be modified.
     * @param B The quaternion to divide by.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32& operator /= (Abytek::F_Matrix2x2_F32& A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        A *= Abytek::Inverse(B);
        return A;
    }
    /**
     * @brief Division operator for a 3x3 matrix and a quaternion.
     * @param A The matrix.
     * @param B The quaternion.
     * @return The result of A * inverse(B).
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator / (Abytek::PA_Matrix3x3_F32 A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        return A * Abytek::Inverse(B);
    }
    /**
     * @brief Compound division operator for a 3x3 matrix and a quaternion.
     * @param A The matrix to be modified.
     * @param B The quaternion to divide by.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32& operator /= (Abytek::F_Matrix3x3_F32& A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        A *= Abytek::Inverse(B);
        return A;
    }
    /**
     * @brief Division operator for a 4x4 matrix and a quaternion.
     * @param A The matrix.
     * @param B The quaternion.
     * @return The result of A * inverse(B).
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator / (Abytek::PA_Matrix4x4_F32 A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        return A * Abytek::Inverse(B);
    }
    /**
     * @brief Compound division operator for a 4x4 matrix and a quaternion.
     * @param A The matrix to be modified.
     * @param B The quaternion to divide by.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32& operator /= (Abytek::F_Matrix4x4_F32& A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        A *= Abytek::Inverse(B);
        return A;
    }
}