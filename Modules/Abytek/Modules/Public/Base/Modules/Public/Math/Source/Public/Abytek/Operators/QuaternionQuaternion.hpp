/**
 * @file QuaternionQuaternion.hpp
 * @brief Defines operations between quaternions.
 * 
 * This file contains operator overloads for interactions between quaternions:
 * - Binary operations (+, -, *, /) between quaternions
 * - Comparison operations (==, !=)
 * - Compound assignment operations (+=, -=, *=, /=)
 * 
 * All operations are defined for F32 (floating point) quaternions.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data.hpp"
#include "Abytek/Functions/QuaternionInverse.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek::Internal::MathOperators
{
    /**
     * @brief Addition operator for two quaternions.
     * @param A The first quaternion.
     * @param B The second quaternion.
     * @return The sum of the quaternions.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32 operator + (Abytek::PA_Quaternion_F32 A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        return Abytek::H_Data4::Add<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
    }
    /**
     * @brief Compound addition operator for two quaternions.
     * @param A The quaternion to be modified.
     * @param B The quaternion to add.
     * @return Reference to the modified quaternion.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32& operator += (Abytek::F_Quaternion_F32& A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        A = Abytek::H_Data4::Add<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
        return A;
    }
    /**
     * @brief Subtraction operator for two quaternions.
     * @param A The first quaternion.
     * @param B The second quaternion.
     * @return The difference of the quaternions.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32 operator - (Abytek::PA_Quaternion_F32 A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        return Abytek::H_Data4::Subtract<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
    }
    /**
     * @brief Compound subtraction operator for two quaternions.
     * @param A The quaternion to be modified.
     * @param B The quaternion to subtract.
     * @return Reference to the modified quaternion.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32& operator -= (Abytek::F_Quaternion_F32& A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        A = Abytek::H_Data4::Subtract<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
        return A;
    }
    /**
     * @brief Multiplication operator for two quaternions.
     * @param A The first quaternion.
     * @param B The second quaternion.
     * @return The product of the quaternions (complex multiplication).
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32 operator * (Abytek::PA_Quaternion_F32 A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        return Abytek::H_Data4::ComplexMultiply<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
    }
    /**
     * @brief Compound multiplication operator for two quaternions.
     * @param A The quaternion to be modified.
     * @param B The quaternion to multiply by.
     * @return Reference to the modified quaternion.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32& operator *= (Abytek::F_Quaternion_F32& A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        A = Abytek::H_Data4::ComplexMultiply<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
        return A;
    }
    /**
     * @brief Division operator for two quaternions.
     * @param A The first quaternion.
     * @param B The second quaternion.
     * @return The result of A * inverse(B).
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32 operator / (Abytek::PA_Quaternion_F32 A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        return Abytek::H_Data4::ComplexMultiply<Abytek::F32, Abytek::F_QuaternionConfig>(A, Abytek::Inverse(B));
    }
    /**
     * @brief Compound division operator for two quaternions.
     * @param A The quaternion to be modified.
     * @param B The quaternion to divide by.
     * @return Reference to the modified quaternion.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32& operator /= (Abytek::F_Quaternion_F32& A, Abytek::PA_Quaternion_F32 B) noexcept
    {
        A = Abytek::H_Data4::ComplexMultiply<Abytek::F32, Abytek::F_QuaternionConfig>(A, Abytek::Inverse(B));
        return A;
    }
}