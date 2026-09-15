#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"

/**
 * @file ACos.hpp
 * @brief Provides arc cosine functions for various types.
 * 
 * This file contains overloaded ACos functions that calculate the arc cosine (inverse cosine) 
 * of input values across different numeric types, vectors, matrices, and quaternions.
 * Returns angles in radians in the range [0, π].
 */
namespace Abytek
{
    /**
     * @brief Calculates the component-wise arc cosine of a 2D float vector.
     * @param A The input vector with components in range [-1, 1].
     * @return A vector with the arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 ACos(PA_Vector2_F32 A) noexcept
    {
        return {
            ACos(A.X),
            ACos(A.Y)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc cosine of a 3D float vector.
     * @param A The input vector with components in range [-1, 1].
     * @return A vector with the arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 ACos(PA_Vector3_F32 A) noexcept
    {
        return {
            ACos(A.X),
            ACos(A.Y),
            ACos(A.Z)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc cosine of a 4D float vector.
     * @param A The input vector with components in range [-1, 1].
     * @return A vector with the arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 ACos(PA_Vector4_F32 A) noexcept
    {
        return {
            ACos(A.X),
            ACos(A.Y),
            ACos(A.Z),
            ACos(A.W)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc cosine of a 2x2 float matrix.
     * @param A The input matrix with components in range [-1, 1].
     * @return A matrix with the arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 ACos(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            ACos(A.A),
            ACos(A.B)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc cosine of a 3x3 float matrix.
     * @param A The input matrix with components in range [-1, 1].
     * @return A matrix with the arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 ACos(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            ACos(A.A),
            ACos(A.B),
            ACos(A.C)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc cosine of a 4x4 float matrix.
     * @param A The input matrix with components in range [-1, 1].
     * @return A matrix with the arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 ACos(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            ACos(A.A),
            ACos(A.B),
            ACos(A.C),
            ACos(A.D)
        };
    }
    
    /**
     * @brief Calculates the component-wise arc cosine of a float quaternion.
     * @param A The input quaternion with components in range [-1, 1].
     * @return A quaternion with the arc cosine of each component.
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 ACos(PA_Quaternion_F32 A) noexcept
    {
        return {
            ACos(A.X),
            ACos(A.Y),
            ACos(A.Z),
            ACos(A.W)
        };
    }
}