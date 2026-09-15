#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"

/**
 * @file ASin.hpp
 * @brief Provides arc sine functions for various types.
 * 
 * This file contains overloaded ASin functions that calculate the arc sine (inverse sine) 
 * of input values across different numeric types, vectors, matrices, and quaternions.
 * Returns angles in radians in the range [-π/2, π/2].
 */
namespace Abytek
{
    /**
     * @brief Calculates the component-wise arc sine of a 2D float vector.
     * @param A The input vector with components in range [-1, 1].
     * @return A vector with the arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 ASin(PA_Vector2_F32 A) noexcept
    {
        return {
            ASin(A.X),
            ASin(A.Y)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc sine of a 3D float vector.
     * @param A The input vector with components in range [-1, 1].
     * @return A vector with the arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 ASin(PA_Vector3_F32 A) noexcept
    {
        return {
            ASin(A.X),
            ASin(A.Y),
            ASin(A.Z)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc sine of a 4D float vector.
     * @param A The input vector with components in range [-1, 1].
     * @return A vector with the arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 ASin(PA_Vector4_F32 A) noexcept
    {
        return {
            ASin(A.X),
            ASin(A.Y),
            ASin(A.Z),
            ASin(A.W)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc sine of a 2x2 float matrix.
     * @param A The input matrix with components in range [-1, 1].
     * @return A matrix with the arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 ASin(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            ASin(A.A),
            ASin(A.B)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc sine of a 3x3 float matrix.
     * @param A The input matrix with components in range [-1, 1].
     * @return A matrix with the arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 ASin(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            ASin(A.A),
            ASin(A.B),
            ASin(A.C)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc sine of a 4x4 float matrix.
     * @param A The input matrix with components in range [-1, 1].
     * @return A matrix with the arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 ASin(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            ASin(A.A),
            ASin(A.B),
            ASin(A.C),
            ASin(A.D)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc sine of a float quaternion.
     * @param A The input quaternion with components in range [-1, 1].
     * @return A quaternion with the arc sine of each component.
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 ASin(PA_Quaternion_F32 A) noexcept
    {
        return {
            ASin(A.X),
            ASin(A.Y),
            ASin(A.Z),
            ASin(A.W)
        };
    }
}