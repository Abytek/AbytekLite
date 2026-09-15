/**
 * @file Tan.hpp
 * @brief Functions for calculating the tangent of angle values.
 * 
 * Provides optimized, inlined tangent functions for various numeric types and vectors.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the component-wise tangent of a 2D float vector.
     * @param A The vector containing angles in radians.
     * @return A vector containing the tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Tan(PA_Vector2_F32 A) noexcept
    {
        return {
            Tan(A.X),
            Tan(A.Y)
        };
    }
    
    /**
     * @brief Calculates the component-wise tangent of a 3D float vector.
     * @param A The vector containing angles in radians.
     * @return A vector containing the tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Tan(PA_Vector3_F32 A) noexcept
    {
        return {
            Tan(A.X),
            Tan(A.Y),
            Tan(A.Z)
        };
    }
    
    /**
     * @brief Calculates the component-wise tangent of a 4D float vector.
     * @param A The vector containing angles in radians.
     * @return A vector containing the tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Tan(PA_Vector4_F32 A) noexcept
    {
        return {
            Tan(A.X),
            Tan(A.Y),
            Tan(A.Z),
            Tan(A.W)
        };
    }
    
    /**
     * @brief Calculates the component-wise tangent of a 2x2 float matrix.
     * @param A The matrix containing angles in radians.
     * @return A matrix containing the tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Tan(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Tan(A.A),
            Tan(A.B)
        };
    }
    
    /**
     * @brief Calculates the component-wise tangent of a 3x3 float matrix.
     * @param A The matrix containing angles in radians.
     * @return A matrix containing the tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Tan(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Tan(A.A),
            Tan(A.B),
            Tan(A.C)
        };
    }
    
    /**
     * @brief Calculates the component-wise tangent of a 4x4 float matrix.
     * @param A The matrix containing angles in radians.
     * @return A matrix containing the tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Tan(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Tan(A.A),
            Tan(A.B),
            Tan(A.C),
            Tan(A.D)
        };
    }
    
    /**
     * @brief Calculates the component-wise tangent of a quaternion.
     * @param A The quaternion containing angles in radians.
     * @return A quaternion containing the tangent of each component.
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Tan(PA_Quaternion_F32 A) noexcept
    {
        return {
            Tan(A.X),
            Tan(A.Y),
            Tan(A.Z),
            Tan(A.W)
        };
    }
}