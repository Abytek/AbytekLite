/**
 * @file MatrixMatrix_F32.hpp
 * @brief Defines matrix-matrix operations for 32-bit floating point type (F32).
 * 
 * This file contains operator overloads for F32 matrix types including:
 * - Unary negation (-)
 * - Binary operations (+, -, *, /) between matrices
 * - Binary operations (+, -, *, /) between a matrix and a scalar
 * - Comparison operations (==, !=)
 * - Compound assignment operations (+=, -=, *=, /=)
 * 
 * Matrix operations are defined for 2x2, 3x3, and 4x4 matrices.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data.hpp"
#include "Abytek/Functions/MatrixInverse.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Operators/MatrixVector.hpp"


namespace Abytek::Internal::MathOperators
{
    /**
     * @brief Unary negation operator for 2x2 F32 matrices.
     * @param A The matrix to negate.
     * @return The negated matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator - (Abytek::PA_Matrix2x2_F32 A) noexcept
    {
        return Abytek::H_Data2x2::Minus<Abytek::F32, Abytek::F_VecmaConfig>(A);
    }

    /**
     * @brief Addition operator for two 2x2 F32 matrices.
     * @param A The first matrix.
     * @param B The second matrix.
     * @return The sum of the matrices.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator + (Abytek::PA_Matrix2x2_F32 A, Abytek::PA_Matrix2x2_F32 B) noexcept
    {
        return Abytek::H_Data2x2::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound addition operator for two 2x2 F32 matrices.
     * @param A The matrix to be modified.
     * @param B The matrix to add.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32& operator += (Abytek::F_Matrix2x2_F32& A, Abytek::PA_Matrix2x2_F32 B) noexcept
    {
        A = Abytek::H_Data2x2::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Subtraction operator for two 2x2 F32 matrices.
     * @param A The first matrix.
     * @param B The second matrix.
     * @return The difference of the matrices.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator - (Abytek::PA_Matrix2x2_F32 A, Abytek::PA_Matrix2x2_F32 B) noexcept
    {
        return Abytek::H_Data2x2::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound subtraction operator for two 2x2 F32 matrices.
     * @param A The matrix to be modified.
     * @param B The matrix to subtract.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32& operator -= (Abytek::F_Matrix2x2_F32& A, Abytek::PA_Matrix2x2_F32 B) noexcept
    {
        A = Abytek::H_Data2x2::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Multiplication operator for two 2x2 F32 matrices.
     * @param A The first matrix.
     * @param B The second matrix.
     * @return The product of the matrices.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator * (Abytek::PA_Matrix2x2_F32 A, Abytek::PA_Matrix2x2_F32 B) noexcept
    {
        return {
            A * B.A,
            A * B.B
        };
    }
    /**
     * @brief Compound multiplication operator for two 2x2 F32 matrices.
     * @param A The matrix to be modified.
     * @param B The matrix to multiply by.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32& operator *= (Abytek::F_Matrix2x2_F32& A, Abytek::PA_Matrix2x2_F32 B) noexcept
    {
        A = {
            A * B.A,
            A * B.B
        };
        return A;
    }
    /**
     * @brief Division operator for two 2x2 F32 matrices.
     * @param A The dividend matrix.
     * @param B The divisor matrix.
     * @return The quotient of the matrices (A * inverse(B)).
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator / (Abytek::PA_Matrix2x2_F32 A, Abytek::PA_Matrix2x2_F32 B) noexcept
    {
        return A * Abytek::Inverse(B);
    }
    /**
     * @brief Compound division operator for two 2x2 F32 matrices.
     * @param A The matrix to be modified.
     * @param B The divisor matrix.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32& operator /= (Abytek::F_Matrix2x2_F32& A, Abytek::PA_Matrix2x2_F32 B) noexcept
    {
        A *= Abytek::Inverse(B);
        return A;
    }

    /**
     * @brief Addition operator for a 2x2 F32 matrix and a scalar.
     * @param A The matrix.
     * @param B The scalar.
     * @return The matrix with the scalar added to each element.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator + (Abytek::PA_Matrix2x2_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data2x2::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound addition operator for a 2x2 F32 matrix and a scalar.
     * @param A The matrix to be modified.
     * @param B The scalar to add.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32& operator += (Abytek::F_Matrix2x2_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data2x2::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Subtraction operator for a 2x2 F32 matrix and a scalar.
     * @param A The matrix.
     * @param B The scalar.
     * @return The matrix with the scalar subtracted from each element.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator - (Abytek::PA_Matrix2x2_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data2x2::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound subtraction operator for a 2x2 F32 matrix and a scalar.
     * @param A The matrix to be modified.
     * @param B The scalar to subtract.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32& operator -= (Abytek::F_Matrix2x2_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data2x2::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Multiplication operator for a 2x2 F32 matrix and a scalar.
     * @param A The matrix.
     * @param B The scalar.
     * @return The matrix with each element multiplied by the scalar.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator * (Abytek::PA_Matrix2x2_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data2x2::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound multiplication operator for a 2x2 F32 matrix and a scalar.
     * @param A The matrix to be modified.
     * @param B The scalar to multiply by.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32& operator *= (Abytek::F_Matrix2x2_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data2x2::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Division operator for a 2x2 F32 matrix and a scalar.
     * @param A The matrix.
     * @param B The scalar.
     * @return The matrix with each element divided by the scalar.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32 operator / (Abytek::PA_Matrix2x2_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data2x2::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound division operator for a 2x2 F32 matrix and a scalar.
     * @param A The matrix to be modified.
     * @param B The scalar to divide by.
     * @return Reference to the modified matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix2x2_F32& operator /= (Abytek::F_Matrix2x2_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data2x2::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    /**
     * @brief Unary negation operator for 3x3 F32 matrices.
     * @param A The matrix to negate.
     * @return The negated matrix.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator - (Abytek::PA_Matrix3x3_F32 A) noexcept
    {
        return Abytek::H_Data3x3::Minus<Abytek::F32, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator + (Abytek::PA_Matrix3x3_F32 A, Abytek::PA_Matrix3x3_F32 B) noexcept
    {
        return Abytek::H_Data3x3::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32& operator += (Abytek::F_Matrix3x3_F32& A, Abytek::PA_Matrix3x3_F32 B) noexcept
    {
        A = Abytek::H_Data3x3::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator - (Abytek::PA_Matrix3x3_F32 A, Abytek::PA_Matrix3x3_F32 B) noexcept
    {
        return Abytek::H_Data3x3::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32& operator -= (Abytek::F_Matrix3x3_F32& A, Abytek::PA_Matrix3x3_F32 B) noexcept
    {
        A = Abytek::H_Data3x3::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator * (Abytek::PA_Matrix3x3_F32 A, Abytek::PA_Matrix3x3_F32 B) noexcept
    {
        return {
            A * B.A,
            A * B.B,
            A * B.C
        };
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32& operator *= (Abytek::F_Matrix3x3_F32& A, Abytek::PA_Matrix3x3_F32 B) noexcept
    {
        A = {
            A * B.A,
            A * B.B,
            A * B.C
        };
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator / (Abytek::PA_Matrix3x3_F32 A, Abytek::PA_Matrix3x3_F32 B) noexcept
    {
        return A * Abytek::Inverse(B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32& operator /= (Abytek::F_Matrix3x3_F32& A, Abytek::PA_Matrix3x3_F32 B) noexcept
    {
        A *= Abytek::Inverse(B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator + (Abytek::PA_Matrix3x3_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data3x3::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32& operator += (Abytek::F_Matrix3x3_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data3x3::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator - (Abytek::PA_Matrix3x3_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data3x3::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32& operator -= (Abytek::F_Matrix3x3_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data3x3::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator * (Abytek::PA_Matrix3x3_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data3x3::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32& operator *= (Abytek::F_Matrix3x3_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data3x3::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32 operator / (Abytek::PA_Matrix3x3_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data3x3::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix3x3_F32& operator /= (Abytek::F_Matrix3x3_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data3x3::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator - (Abytek::PA_Matrix4x4_F32 A) noexcept
    {
        return Abytek::H_Data4x4::Minus<Abytek::F32, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator + (Abytek::PA_Matrix4x4_F32 A, Abytek::PA_Matrix4x4_F32 B) noexcept
    {
        return Abytek::H_Data4x4::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32& operator += (Abytek::F_Matrix4x4_F32& A, Abytek::PA_Matrix4x4_F32 B) noexcept
    {
        A = Abytek::H_Data4x4::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator - (Abytek::PA_Matrix4x4_F32 A, Abytek::PA_Matrix4x4_F32 B) noexcept
    {
        return Abytek::H_Data4x4::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32& operator -= (Abytek::F_Matrix4x4_F32& A, Abytek::PA_Matrix4x4_F32 B) noexcept
    {
        A = Abytek::H_Data4x4::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator * (Abytek::PA_Matrix4x4_F32 A, Abytek::PA_Matrix4x4_F32 B) noexcept
    {
        return {
            A * B.A,
            A * B.B,
            A * B.C,
            A * B.D
        };
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32& operator *= (Abytek::F_Matrix4x4_F32& A, Abytek::PA_Matrix4x4_F32 B) noexcept
    {
        A = {
            A * B.A,
            A * B.B,
            A * B.C,
            A * B.D
        };
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator / (Abytek::PA_Matrix4x4_F32 A, Abytek::PA_Matrix4x4_F32 B) noexcept
    {
        return A * Abytek::Inverse(B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32& operator /= (Abytek::F_Matrix4x4_F32& A, Abytek::PA_Matrix4x4_F32 B) noexcept
    {
        A *= Abytek::Inverse(B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator + (Abytek::PA_Matrix4x4_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4x4::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32& operator += (Abytek::F_Matrix4x4_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4x4::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator - (Abytek::PA_Matrix4x4_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4x4::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32& operator -= (Abytek::F_Matrix4x4_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4x4::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator * (Abytek::PA_Matrix4x4_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4x4::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32& operator *= (Abytek::F_Matrix4x4_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4x4::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32 operator / (Abytek::PA_Matrix4x4_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4x4::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Matrix4x4_F32& operator /= (Abytek::F_Matrix4x4_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4x4::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
}