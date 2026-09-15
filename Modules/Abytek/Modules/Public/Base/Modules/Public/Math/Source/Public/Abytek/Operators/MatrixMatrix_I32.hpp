/**
 * @file MatrixMatrix_I32.hpp
 * @brief Defines matrix-matrix operations for 32-bit signed integer type (I32).
 * 
 * This file contains operator overloads for I32 matrix types including:
 * - Unary negation (-)
 * - Binary operations (+, -, *) between matrices
 * - Binary operations (+, -, *) between a matrix and a scalar
 * - Comparison operations (==, !=)
 * - Compound assignment operations (+=, -=, *=)
 * 
 * Matrix operations are defined for 2x2, 3x3, and 4x4 matrices.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Operators/MatrixVector.hpp"


namespace Abytek::Internal::MathOperators
{
    /**
     * @brief Unary negation operator for 2x2 I32 matrices.
     * @param A The matrix to negate.
     * @return The negated matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32 operator - (Abytek::PA_Matrix2x2_I32 A) noexcept
    {
        return Abytek::H_Data2x2::Minus<Abytek::I32, Abytek::F_VecmaConfig>(A);
    }

    /**
     * @brief Addition operator for two 2x2 I32 matrices.
     * @param A The first matrix.
     * @param B The second matrix.
     * @return The sum of the matrices.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32 operator + (Abytek::PA_Matrix2x2_I32 A, Abytek::PA_Matrix2x2_I32 B) noexcept
    {
        return Abytek::H_Data2x2::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound addition operator for two 2x2 I32 matrices.
     * @param A The matrix to be modified.
     * @param B The matrix to add.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32& operator += (Abytek::F_Matrix2x2_I32& A, Abytek::PA_Matrix2x2_I32 B) noexcept
    {
        A = Abytek::H_Data2x2::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Subtraction operator for two 2x2 I32 matrices.
     * @param A The first matrix.
     * @param B The second matrix.
     * @return The difference of the matrices.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32 operator - (Abytek::PA_Matrix2x2_I32 A, Abytek::PA_Matrix2x2_I32 B) noexcept
    {
        return Abytek::H_Data2x2::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound subtraction operator for two 2x2 I32 matrices.
     * @param A The matrix to be modified.
     * @param B The matrix to subtract.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32& operator -= (Abytek::F_Matrix2x2_I32& A, Abytek::PA_Matrix2x2_I32 B) noexcept
    {
        A = Abytek::H_Data2x2::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Multiplication operator for two 2x2 I32 matrices.
     * @param A The first matrix.
     * @param B The second matrix.
     * @return The product of the matrices.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32 operator * (Abytek::PA_Matrix2x2_I32 A, Abytek::PA_Matrix2x2_I32 B) noexcept
    {
        return {
            A * B.A,
            A * B.B
        };
    }
    /**
     * @brief Compound multiplication operator for two 2x2 I32 matrices.
     * @param A The matrix to be modified.
     * @param B The matrix to multiply by.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32& operator *= (Abytek::F_Matrix2x2_I32& A, Abytek::PA_Matrix2x2_I32 B) noexcept
    {
        A = {
            A * B.A,
            A * B.B
        };
        return A;
    }

    /**
     * @brief Addition operator for a 2x2 I32 matrix and a scalar.
     * @param A The matrix.
     * @param B The scalar.
     * @return The matrix with the scalar added to each element.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32 operator + (Abytek::PA_Matrix2x2_I32 A, Abytek::I32 B) noexcept
    {
        return Abytek::H_Data2x2::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound addition operator for a 2x2 I32 matrix and a scalar.
     * @param A The matrix to be modified.
     * @param B The scalar to add.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32& operator += (Abytek::F_Matrix2x2_I32& A, Abytek::I32 B) noexcept
    {
        A = Abytek::H_Data2x2::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Subtraction operator for a 2x2 I32 matrix and a scalar.
     * @param A The matrix.
     * @param B The scalar.
     * @return The matrix with the scalar subtracted from each element.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32 operator - (Abytek::PA_Matrix2x2_I32 A, Abytek::I32 B) noexcept
    {
        return Abytek::H_Data2x2::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound subtraction operator for a 2x2 I32 matrix and a scalar.
     * @param A The matrix to be modified.
     * @param B The scalar to subtract.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32& operator -= (Abytek::F_Matrix2x2_I32& A, Abytek::I32 B) noexcept
    {
        A = Abytek::H_Data2x2::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Multiplication operator for a 2x2 I32 matrix and a scalar.
     * @param A The matrix.
     * @param B The scalar.
     * @return The matrix with each element multiplied by the scalar.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32 operator * (Abytek::PA_Matrix2x2_I32 A, Abytek::I32 B) noexcept
    {
        return Abytek::H_Data2x2::Multiply<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound multiplication operator for a 2x2 I32 matrix and a scalar.
     * @param A The matrix to be modified.
     * @param B The scalar to multiply by.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_I32& operator *= (Abytek::F_Matrix2x2_I32& A, Abytek::I32 B) noexcept
    {
        A = Abytek::H_Data2x2::Multiply<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    /**
     * @brief Unary negation operator for 3x3 I32 matrices.
     * @param A The matrix to negate.
     * @return The negated matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32 operator - (Abytek::PA_Matrix3x3_I32 A) noexcept
    {
        return Abytek::H_Data3x3::Minus<Abytek::I32, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32 operator + (Abytek::PA_Matrix3x3_I32 A, Abytek::PA_Matrix3x3_I32 B) noexcept
    {
        return Abytek::H_Data3x3::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32& operator += (Abytek::F_Matrix3x3_I32& A, Abytek::PA_Matrix3x3_I32 B) noexcept
    {
        A = Abytek::H_Data3x3::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32 operator - (Abytek::PA_Matrix3x3_I32 A, Abytek::PA_Matrix3x3_I32 B) noexcept
    {
        return Abytek::H_Data3x3::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32& operator -= (Abytek::F_Matrix3x3_I32& A, Abytek::PA_Matrix3x3_I32 B) noexcept
    {
        A = Abytek::H_Data3x3::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32 operator * (Abytek::PA_Matrix3x3_I32 A, Abytek::PA_Matrix3x3_I32 B) noexcept
    {
        return {
            A * B.A,
            A * B.B,
            A * B.C
        };
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32& operator *= (Abytek::F_Matrix3x3_I32& A, Abytek::PA_Matrix3x3_I32 B) noexcept
    {
        A = {
            A * B.A,
            A * B.B,
            A * B.C
        };
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32 operator + (Abytek::PA_Matrix3x3_I32 A, Abytek::I32 B) noexcept
    {
        return Abytek::H_Data3x3::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32& operator += (Abytek::F_Matrix3x3_I32& A, Abytek::I32 B) noexcept
    {
        A = Abytek::H_Data3x3::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32 operator - (Abytek::PA_Matrix3x3_I32 A, Abytek::I32 B) noexcept
    {
        return Abytek::H_Data3x3::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32& operator -= (Abytek::F_Matrix3x3_I32& A, Abytek::I32 B) noexcept
    {
        A = Abytek::H_Data3x3::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32 operator * (Abytek::PA_Matrix3x3_I32 A, Abytek::I32 B) noexcept
    {
        return Abytek::H_Data3x3::Multiply<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_I32& operator *= (Abytek::F_Matrix3x3_I32& A, Abytek::I32 B) noexcept
    {
        A = Abytek::H_Data3x3::Multiply<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32 operator - (Abytek::PA_Matrix4x4_I32 A) noexcept
    {
        return Abytek::H_Data4x4::Minus<Abytek::I32, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32 operator + (Abytek::PA_Matrix4x4_I32 A, Abytek::PA_Matrix4x4_I32 B) noexcept
    {
        return Abytek::H_Data4x4::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32& operator += (Abytek::F_Matrix4x4_I32& A, Abytek::PA_Matrix4x4_I32 B) noexcept
    {
        A = Abytek::H_Data4x4::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32 operator - (Abytek::PA_Matrix4x4_I32 A, Abytek::PA_Matrix4x4_I32 B) noexcept
    {
        return Abytek::H_Data4x4::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32& operator -= (Abytek::F_Matrix4x4_I32& A, Abytek::PA_Matrix4x4_I32 B) noexcept
    {
        A = Abytek::H_Data4x4::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32 operator * (Abytek::PA_Matrix4x4_I32 A, Abytek::PA_Matrix4x4_I32 B) noexcept
    {
        return {
            A * B.A,
            A * B.B,
            A * B.C,
            A * B.D
        };
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32& operator *= (Abytek::F_Matrix4x4_I32& A, Abytek::PA_Matrix4x4_I32 B) noexcept
    {
        A = {
            A * B.A,
            A * B.B,
            A * B.C,
            A * B.D
        };
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32 operator + (Abytek::PA_Matrix4x4_I32 A, Abytek::I32 B) noexcept
    {
        return Abytek::H_Data4x4::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32& operator += (Abytek::F_Matrix4x4_I32& A, Abytek::I32 B) noexcept
    {
        A = Abytek::H_Data4x4::Add<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32 operator - (Abytek::PA_Matrix4x4_I32 A, Abytek::I32 B) noexcept
    {
        return Abytek::H_Data4x4::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32& operator -= (Abytek::F_Matrix4x4_I32& A, Abytek::I32 B) noexcept
    {
        A = Abytek::H_Data4x4::Subtract<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32 operator * (Abytek::PA_Matrix4x4_I32 A, Abytek::I32 B) noexcept
    {
        return Abytek::H_Data4x4::Multiply<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_I32& operator *= (Abytek::F_Matrix4x4_I32& A, Abytek::I32 B) noexcept
    {
        A = Abytek::H_Data4x4::Multiply<Abytek::I32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
}