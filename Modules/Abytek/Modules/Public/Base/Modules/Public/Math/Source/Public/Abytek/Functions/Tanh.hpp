/**
 * @file Tanh.hpp
 * @brief Functions for calculating the hyperbolic tangent of values.
 * 
 * Provides optimized, inlined hyperbolic tangent functions for various numeric types and vectors.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the component-wise hyperbolic tangent of a 2D float vector.
     * @param A The vector to calculate the hyperbolic tangent of.
     * @return A vector containing the hyperbolic tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Tanh(PA_Vector2_F32 A) noexcept
    {
        return {
            Tanh(A.X),
            Tanh(A.Y)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic tangent of a 3D float vector.
     * @param A The vector to calculate the hyperbolic tangent of.
     * @return A vector containing the hyperbolic tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Tanh(PA_Vector3_F32 A) noexcept
    {
        return {
            Tanh(A.X),
            Tanh(A.Y),
            Tanh(A.Z)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic tangent of a 4D float vector.
     * @param A The vector to calculate the hyperbolic tangent of.
     * @return A vector containing the hyperbolic tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Tanh(PA_Vector4_F32 A) noexcept
    {
        return {
            Tanh(A.X),
            Tanh(A.Y),
            Tanh(A.Z),
            Tanh(A.W)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic tangent of a 2x2 float matrix.
     * @param A The matrix to calculate the hyperbolic tangent of.
     * @return A matrix containing the hyperbolic tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Tanh(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Tanh(A.A),
            Tanh(A.B)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic tangent of a 3x3 float matrix.
     * @param A The matrix to calculate the hyperbolic tangent of.
     * @return A matrix containing the hyperbolic tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Tanh(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Tanh(A.A),
            Tanh(A.B),
            Tanh(A.C)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic tangent of a 4x4 float matrix.
     * @param A The matrix to calculate the hyperbolic tangent of.
     * @return A matrix containing the hyperbolic tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Tanh(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Tanh(A.A),
            Tanh(A.B),
            Tanh(A.C),
            Tanh(A.D)
        };
    }
    
    /**
     * @brief Calculates the component-wise hyperbolic tangent of a quaternion.
     * @param A The quaternion to calculate the hyperbolic tangent of.
     * @return A quaternion containing the hyperbolic tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Tanh(PA_Quaternion_F32 A) noexcept
    {
        return {
            Tanh(A.X),
            Tanh(A.Y),
            Tanh(A.Z),
            Tanh(A.W)
        };
    }
}