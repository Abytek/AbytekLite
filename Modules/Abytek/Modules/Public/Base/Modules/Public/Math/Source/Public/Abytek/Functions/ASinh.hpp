#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"

/**
 * @file ASinh.hpp
 * @brief Provides hyperbolic arc sine functions for various types.
 * 
 * This file contains overloaded ASinh functions that calculate the hyperbolic arc sine (inverse hyperbolic sine)
 * of input values across different numeric types, vectors, matrices, and quaternions.
 * The hyperbolic arc sine is defined for all real inputs.
 */
namespace Abytek
{
    /**
     * @brief Calculates the component-wise hyperbolic arc sine of a 2D float vector.
     * @param A The input vector.
     * @return A vector with the hyperbolic arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 ASinh(PA_Vector2_F32 A) noexcept
    {
        return {
            ASinh(A.X),
            ASinh(A.Y)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc sine of a 3D float vector.
     * @param A The input vector.
     * @return A vector with the hyperbolic arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 ASinh(PA_Vector3_F32 A) noexcept
    {
        return {
            ASinh(A.X),
            ASinh(A.Y),
            ASinh(A.Z)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc sine of a 4D float vector.
     * @param A The input vector.
     * @return A vector with the hyperbolic arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 ASinh(PA_Vector4_F32 A) noexcept
    {
        return {
            ASinh(A.X),
            ASinh(A.Y),
            ASinh(A.Z),
            ASinh(A.W)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc sine of a 2x2 float matrix.
     * @param A The input matrix.
     * @return A matrix with the hyperbolic arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 ASinh(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            ASinh(A.A),
            ASinh(A.B)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc sine of a 3x3 float matrix.
     * @param A The input matrix.
     * @return A matrix with the hyperbolic arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 ASinh(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            ASinh(A.A),
            ASinh(A.B),
            ASinh(A.C)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc sine of a 4x4 float matrix.
     * @param A The input matrix.
     * @return A matrix with the hyperbolic arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 ASinh(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            ASinh(A.A),
            ASinh(A.B),
            ASinh(A.C),
            ASinh(A.D)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic arc sine of a float quaternion.
     * @param A The input quaternion.
     * @return A quaternion with the hyperbolic arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 ASinh(PA_Quaternion_F32 A) noexcept
    {
        return {
            ASinh(A.X),
            ASinh(A.Y),
            ASinh(A.Z),
            ASinh(A.W)
        };
    }
}