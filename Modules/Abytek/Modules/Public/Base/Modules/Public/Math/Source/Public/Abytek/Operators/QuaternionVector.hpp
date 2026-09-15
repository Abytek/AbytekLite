/**
 * @file QuaternionVector.hpp
 * @brief Defines operations between quaternions and vectors.
 * 
 * This file contains operator overloads for interactions between quaternions and vectors:
 * - Multiplication of quaternions and vectors (quaternion * vector)
 * 
 * These operations implement rotation of vectors by quaternions, which is a common
 * use case in 3D graphics and physics. The implementation uses quaternion conjugation
 * to perform the rotation.
 * 
 * Operations are defined for 2D, 3D, and 4D vectors with F32 (floating point) quaternions.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data.hpp"
#include "Abytek/Functions/QuaternionInverse.hpp"
#include "Abytek/Functions/QuaternionConjugate.hpp"
#include "Abytek/Types/Quaternion.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek::Internal::MathOperators
{
    /**
     * @brief Multiplication operator for a quaternion and a 2D vector.
     * @param A The quaternion representing rotation.
     * @param B The 2D vector to rotate.
     * @return The rotated 2D vector.
     * 
     * This implements the quaternion rotation formula q * v * q^(-1) for a 2D vector
     * by extending it to a 3D vector with z=0 and extracting the x,y components of the result.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_F32 operator * (Abytek::PA_Quaternion_F32 A, Abytek::PA_Vector2_F32 B) noexcept
    {
        return Abytek::H_Data4::ComplexMultiply<Abytek::F32, Abytek::F_VecmaConfig>(
            A.RebindConfig<Abytek::F_VecmaConfig>(),
            Abytek::H_Data4::ComplexMultiply<Abytek::F32, Abytek::F_VecmaConfig>(
                Abytek::F_Vector4_F32 {
                    B,
                    0.0f,
                    0.0f
                },
                Abytek::Conjugate(A).RebindConfig<Abytek::F_VecmaConfig>()
            )
        ).StaticPermute<0, 1>();
    }

    /**
     * @brief Multiplication operator for a quaternion and a 3D vector.
     * @param A The quaternion representing rotation.
     * @param B The 3D vector to rotate.
     * @return The rotated 3D vector.
     * 
     * This implements the quaternion rotation formula q * v * q^(-1) for a 3D vector
     * by extending it to a 4D vector with w=0 and extracting the x,y,z components of the result.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_F32 operator * (Abytek::PA_Quaternion_F32 A, Abytek::PA_Vector3_F32 B) noexcept
    {
        return Abytek::H_Data4::ComplexMultiply<Abytek::F32, Abytek::F_VecmaConfig>(
            A.RebindConfig<Abytek::F_VecmaConfig>(),
            Abytek::H_Data4::ComplexMultiply<Abytek::F32, Abytek::F_VecmaConfig>(
                Abytek::F_Vector4_F32 {
                    B,
                    0.0f
                },
                Abytek::Conjugate(A).RebindConfig<Abytek::F_VecmaConfig>()
            )
        ).StaticPermute<0, 1, 2>();
    }

    /**
     * @brief Multiplication operator for a quaternion and a 4D vector.
     * @param A The quaternion representing rotation.
     * @param B The 4D vector to rotate.
     * @return The rotated 4D vector.
     * 
     * This implements the quaternion rotation formula q * v * q^(-1) for a 4D vector.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_F32 operator * (Abytek::PA_Quaternion_F32 A, Abytek::PA_Vector4_F32 B) noexcept
    {
        return Abytek::H_Data4::ComplexMultiply<Abytek::F32, Abytek::F_VecmaConfig>(
            A.RebindConfig<Abytek::F_VecmaConfig>(),
            Abytek::H_Data4::ComplexMultiply<Abytek::F32, Abytek::F_VecmaConfig>(
                B,
                Abytek::Conjugate(A).RebindConfig<Abytek::F_VecmaConfig>()
            )
        );
    }
}