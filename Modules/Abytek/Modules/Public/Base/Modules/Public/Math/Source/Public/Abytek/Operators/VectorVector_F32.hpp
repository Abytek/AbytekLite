/**
 * @file VectorVector_F32.hpp
 * @brief Defines vector-vector operations for 32-bit floating point type (F32).
 * 
 * This file contains operator overloads for F32 vector types including:
 * - Unary negation (-)
 * - Binary operations (+, -, *, /) between vectors
 * - Binary operations (+, -, *, /) between a vector and a scalar
 * - Comparison operations (==, !=)
 * - Compound assignment operations (+=, -=, *=, /=)
 * 
 * Vector operations are defined for 2D, 3D, and 4D vectors.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek::Internal::MathOperators
{
    /**
     * @brief Unary negation operator for 2D F32 vectors.
     * @param A The vector to negate.
     * @return The negated vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32 operator - (Abytek::PA_Vector2_F32 A) noexcept
    {
        return Abytek::H_Data2::Minus<Abytek::F32, Abytek::F_VecmaConfig>(A);
    }

    /**
     * @brief Addition operator for two 2D F32 vectors.
     * @param A The first vector.
     * @param B The second vector.
     * @return The sum of the vectors.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32 operator + (Abytek::PA_Vector2_F32 A, Abytek::PA_Vector2_F32 B) noexcept
    {
        return Abytek::H_Data2::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound addition operator for two 2D F32 vectors.
     * @param A The vector to be modified.
     * @param B The vector to add.
     * @return Reference to the modified vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32& operator += (Abytek::F_Vector2_F32& A, Abytek::PA_Vector2_F32 B) noexcept
    {
        A = Abytek::H_Data2::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Subtraction operator for two 2D F32 vectors.
     * @param A The first vector.
     * @param B The second vector.
     * @return The difference of the vectors.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32 operator - (Abytek::PA_Vector2_F32 A, Abytek::PA_Vector2_F32 B) noexcept
    {
        return Abytek::H_Data2::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound subtraction operator for two 2D F32 vectors.
     * @param A The vector to be modified.
     * @param B The vector to subtract.
     * @return Reference to the modified vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32& operator -= (Abytek::F_Vector2_F32& A, Abytek::PA_Vector2_F32 B) noexcept
    {
        A = Abytek::H_Data2::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Component-wise multiplication operator for two 2D F32 vectors.
     * @param A The first vector.
     * @param B The second vector.
     * @return The component-wise product of the vectors.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32 operator * (Abytek::PA_Vector2_F32 A, Abytek::PA_Vector2_F32 B) noexcept
    {
        return Abytek::H_Data2::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound component-wise multiplication operator for two 2D F32 vectors.
     * @param A The vector to be modified.
     * @param B The vector to multiply by.
     * @return Reference to the modified vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32& operator *= (Abytek::F_Vector2_F32& A, Abytek::PA_Vector2_F32 B) noexcept
    {
        A = Abytek::H_Data2::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Component-wise division operator for two 2D F32 vectors.
     * @param A The first vector.
     * @param B The second vector.
     * @return The component-wise quotient of the vectors.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32 operator / (Abytek::PA_Vector2_F32 A, Abytek::PA_Vector2_F32 B) noexcept
    {
        return Abytek::H_Data2::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound component-wise division operator for two 2D F32 vectors.
     * @param A The vector to be modified.
     * @param B The vector to divide by.
     * @return Reference to the modified vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32& operator /= (Abytek::F_Vector2_F32& A, Abytek::PA_Vector2_F32 B) noexcept
    {
        A = Abytek::H_Data2::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    /**
     * @brief Addition operator for a 2D F32 vector and a scalar.
     * @param A The vector.
     * @param B The scalar.
     * @return The vector with the scalar added to each component.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32 operator + (Abytek::PA_Vector2_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data2::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound addition operator for a 2D F32 vector and a scalar.
     * @param A The vector to be modified.
     * @param B The scalar to add.
     * @return Reference to the modified vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32& operator += (Abytek::F_Vector2_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data2::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Subtraction operator for a 2D F32 vector and a scalar.
     * @param A The vector.
     * @param B The scalar.
     * @return The vector with the scalar subtracted from each component.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32 operator - (Abytek::PA_Vector2_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data2::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound subtraction operator for a 2D F32 vector and a scalar.
     * @param A The vector to be modified.
     * @param B The scalar to subtract.
     * @return Reference to the modified vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32& operator -= (Abytek::F_Vector2_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data2::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Multiplication operator for a 2D F32 vector and a scalar.
     * @param A The vector.
     * @param B The scalar.
     * @return The vector with each component multiplied by the scalar.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32 operator * (Abytek::PA_Vector2_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data2::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound multiplication operator for a 2D F32 vector and a scalar.
     * @param A The vector to be modified.
     * @param B The scalar to multiply by.
     * @return Reference to the modified vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32& operator *= (Abytek::F_Vector2_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data2::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    /**
     * @brief Division operator for a 2D F32 vector and a scalar.
     * @param A The vector.
     * @param B The scalar.
     * @return The vector with each component divided by the scalar.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32 operator / (Abytek::PA_Vector2_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data2::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    /**
     * @brief Compound division operator for a 2D F32 vector and a scalar.
     * @param A The vector to be modified.
     * @param B The scalar to divide by.
     * @return Reference to the modified vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32& operator /= (Abytek::F_Vector2_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data2::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    /**
     * @brief Unary negation operator for 3D F32 vectors.
     * @param A The vector to negate.
     * @return The negated vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32 operator - (Abytek::PA_Vector3_F32 A) noexcept
    {
        return Abytek::H_Data3::Minus<Abytek::F32, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32 operator + (Abytek::PA_Vector3_F32 A, Abytek::PA_Vector3_F32 B) noexcept
    {
        return Abytek::H_Data3::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32& operator += (Abytek::F_Vector3_F32& A, Abytek::PA_Vector3_F32 B) noexcept
    {
        A = Abytek::H_Data3::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32 operator - (Abytek::PA_Vector3_F32 A, Abytek::PA_Vector3_F32 B) noexcept
    {
        return Abytek::H_Data3::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32& operator -= (Abytek::F_Vector3_F32& A, Abytek::PA_Vector3_F32 B) noexcept
    {
        A = Abytek::H_Data3::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32 operator * (Abytek::PA_Vector3_F32 A, Abytek::PA_Vector3_F32 B) noexcept
    {
        return Abytek::H_Data3::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32& operator *= (Abytek::F_Vector3_F32& A, Abytek::PA_Vector3_F32 B) noexcept
    {
        A = Abytek::H_Data3::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32 operator / (Abytek::PA_Vector3_F32 A, Abytek::PA_Vector3_F32 B) noexcept
    {
        return Abytek::H_Data3::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32& operator /= (Abytek::F_Vector3_F32& A, Abytek::PA_Vector3_F32 B) noexcept
    {
        A = Abytek::H_Data3::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32 operator + (Abytek::PA_Vector3_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data3::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32& operator += (Abytek::F_Vector3_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data3::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32 operator - (Abytek::PA_Vector3_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data3::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32& operator -= (Abytek::F_Vector3_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data3::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32 operator * (Abytek::PA_Vector3_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data3::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32& operator *= (Abytek::F_Vector3_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data3::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32 operator / (Abytek::PA_Vector3_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data3::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32& operator /= (Abytek::F_Vector3_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data3::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32 operator - (Abytek::PA_Vector4_F32 A) noexcept
    {
        return Abytek::H_Data4::Minus<Abytek::F32, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32 operator + (Abytek::PA_Vector4_F32 A, Abytek::PA_Vector4_F32 B) noexcept
    {
        return Abytek::H_Data4::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32& operator += (Abytek::F_Vector4_F32& A, Abytek::PA_Vector4_F32 B) noexcept
    {
        A = Abytek::H_Data4::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32 operator - (Abytek::PA_Vector4_F32 A, Abytek::PA_Vector4_F32 B) noexcept
    {
        return Abytek::H_Data4::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32& operator -= (Abytek::F_Vector4_F32& A, Abytek::PA_Vector4_F32 B) noexcept
    {
        A = Abytek::H_Data4::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32 operator * (Abytek::PA_Vector4_F32 A, Abytek::PA_Vector4_F32 B) noexcept
    {
        return Abytek::H_Data4::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32& operator *= (Abytek::F_Vector4_F32& A, Abytek::PA_Vector4_F32 B) noexcept
    {
        A = Abytek::H_Data4::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32 operator / (Abytek::PA_Vector4_F32 A, Abytek::PA_Vector4_F32 B) noexcept
    {
        return Abytek::H_Data4::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32& operator /= (Abytek::F_Vector4_F32& A, Abytek::PA_Vector4_F32 B) noexcept
    {
        A = Abytek::H_Data4::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32 operator + (Abytek::PA_Vector4_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32& operator += (Abytek::F_Vector4_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4::Add<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32 operator - (Abytek::PA_Vector4_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32& operator -= (Abytek::F_Vector4_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4::Subtract<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32 operator * (Abytek::PA_Vector4_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32& operator *= (Abytek::F_Vector4_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4::Multiply<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32 operator / (Abytek::PA_Vector4_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32& operator /= (Abytek::F_Vector4_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4::Divide<Abytek::F32, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
}