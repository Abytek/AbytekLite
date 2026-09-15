#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"

/**
 * @file ACosh.hpp
 * @brief Provides hyperbolic arc cosine functions for various types.
 * 
 * This file contains overloaded ACosh functions that calculate the hyperbolic arc cosine (inverse hyperbolic cosine)
 * of input values across different numeric types, vectors, matrices, and quaternions.
 * The hyperbolic arc cosine is defined for inputs >= 1.
 */
namespace Abytek
{
    /**
     * @brief Calculates the component-wise hyperbolic arc cosine of a 2D float vector.
     * @param A The input vector with components >= 1.
     * @return A vector with the hyperbolic arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 ACosh(PA_Vector2_F32 A) noexcept
    {
        return {
            ACosh(A.X),
            ACosh(A.Y)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc cosine of a 3D float vector.
     * @param A The input vector with components >= 1.
     * @return A vector with the hyperbolic arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 ACosh(PA_Vector3_F32 A) noexcept
    {
        return {
            ACosh(A.X),
            ACosh(A.Y),
            ACosh(A.Z)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc cosine of a 4D float vector.
     * @param A The input vector with components >= 1.
     * @return A vector with the hyperbolic arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 ACosh(PA_Vector4_F32 A) noexcept
    {
        return {
            ACosh(A.X),
            ACosh(A.Y),
            ACosh(A.Z),
            ACosh(A.W)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc cosine of a 2x2 float matrix.
     * @param A The input matrix with components >= 1.
     * @return A matrix with the hyperbolic arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 ACosh(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            ACosh(A.A),
            ACosh(A.B)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc cosine of a 3x3 float matrix.
     * @param A The input matrix with components >= 1.
     * @return A matrix with the hyperbolic arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 ACosh(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            ACosh(A.A),
            ACosh(A.B),
            ACosh(A.C)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc cosine of a 4x4 float matrix.
     * @param A The input matrix with components >= 1.
     * @return A matrix with the hyperbolic arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 ACosh(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            ACosh(A.A),
            ACosh(A.B),
            ACosh(A.C),
            ACosh(A.D)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc cosine of a float quaternion.
     * @param A The input quaternion with components >= 1.
     * @return A quaternion with the hyperbolic arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 ACosh(PA_Quaternion_F32 A) noexcept
    {
        return {
            ACosh(A.X),
            ACosh(A.Y),
            ACosh(A.Z),
            ACosh(A.W)
        };
    }
}