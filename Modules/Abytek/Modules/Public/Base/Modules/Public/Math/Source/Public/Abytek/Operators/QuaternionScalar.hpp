/**
 * @file QuaternionScalar.hpp
 * @brief Defines operations between quaternions and scalar values.
 * 
 * This file contains operator overloads for interactions between quaternions and scalar values:
 * - Binary operations (+, -, *, /) between quaternions and scalars
 * - Comparison operations (==, !=) between quaternions and scalars
 * - Compound assignment operations (+=, -=, *=, /=) for quaternions with scalars
 * 
 * All operations are defined for F32 (floating point) quaternions and F32 scalars.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek::Internal::MathOperators
{
    /**
     * @brief Unary negation operator for a quaternion (incorrectly labeled as multiplication).
     * @param A The quaternion to negate.
     * @return The negated quaternion.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32 operator * (Abytek::PA_Quaternion_F32 A) noexcept
    {
        return Abytek::H_Data4::Minus<Abytek::F32, Abytek::F_QuaternionConfig>(A);
    }

    /**
     * @brief Addition operator for a quaternion and a scalar.
     * @param A The quaternion.
     * @param B The scalar.
     * @return The quaternion with the scalar added to each component.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32 operator + (Abytek::PA_Quaternion_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4::Add<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
    }
    /**
     * @brief Compound addition operator for a quaternion and a scalar.
     * @param A The quaternion to be modified.
     * @param B The scalar to add.
     * @return Reference to the modified quaternion.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32& operator += (Abytek::F_Quaternion_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4::Add<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
        return A;
    }
    /**
     * @brief Subtraction operator for a quaternion and a scalar.
     * @param A The quaternion.
     * @param B The scalar.
     * @return The quaternion with the scalar subtracted from each component.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32 operator - (Abytek::PA_Quaternion_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4::Subtract<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
    }
    /**
     * @brief Compound subtraction operator for a quaternion and a scalar.
     * @param A The quaternion to be modified.
     * @param B The scalar to subtract.
     * @return Reference to the modified quaternion.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32& operator -= (Abytek::F_Quaternion_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4::Subtract<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
        return A;
    }
    /**
     * @brief Multiplication operator for a quaternion and a scalar.
     * @param A The quaternion.
     * @param B The scalar.
     * @return The quaternion with each component multiplied by the scalar.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32 operator * (Abytek::PA_Quaternion_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4::Multiply<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
    }
    /**
     * @brief Compound multiplication operator for a quaternion and a scalar.
     * @param A The quaternion to be modified.
     * @param B The scalar to multiply by.
     * @return Reference to the modified quaternion.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32& operator *= (Abytek::F_Quaternion_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4::Multiply<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
        return A;
    }
    /**
     * @brief Division operator for a quaternion and a scalar.
     * @param A The quaternion.
     * @param B The scalar.
     * @return The quaternion with each component divided by the scalar.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32 operator / (Abytek::PA_Quaternion_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4::Divide<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
    }
    /**
     * @brief Compound division operator for a quaternion and a scalar.
     * @param A The quaternion to be modified.
     * @param B The scalar to divide by.
     * @return Reference to the modified quaternion.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32& operator /= (Abytek::F_Quaternion_F32& A, Abytek::F32 B) noexcept
    {
        A = Abytek::H_Data4::Divide<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
        return A;
    }
    /**
     * @brief Equality comparison operator for a quaternion and a scalar.
     * @param A The quaternion.
     * @param B The scalar.
     * @return True if all components of the quaternion equal the scalar, false otherwise.
     */
    ABYTEK_FORCE_INLINE Abytek::B8 operator == (Abytek::PA_Quaternion_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4::Equal<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
    }
    /**
     * @brief Inequality comparison operator for a quaternion and a scalar.
     * @param A The quaternion.
     * @param B The scalar.
     * @return True if any component of the quaternion is not equal to the scalar, false otherwise.
     */
    ABYTEK_FORCE_INLINE Abytek::B8 operator != (Abytek::PA_Quaternion_F32 A, Abytek::F32 B) noexcept
    {
        return Abytek::H_Data4::NotEqual<Abytek::F32, Abytek::F_QuaternionConfig>(A, B);
    }
}