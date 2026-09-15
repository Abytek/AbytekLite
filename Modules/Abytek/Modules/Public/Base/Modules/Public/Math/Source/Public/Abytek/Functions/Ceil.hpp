#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Computes the ceiling of each component of a 2D vector
     * @param A Input vector
     * @return A vector containing the ceiling of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Ceil(PA_Vector2_F32 A) noexcept
    {
        return {
            Ceil(A.X),
            Ceil(A.Y)
        };
    }
    
    /**
     * @brief Computes the ceiling of each component of a 3D vector
     * @param A Input vector
     * @return A vector containing the ceiling of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Ceil(PA_Vector3_F32 A) noexcept
    {
        return {
            Ceil(A.X),
            Ceil(A.Y),
            Ceil(A.Z)
        };
    }
    
    /**
     * @brief Computes the ceiling of each component of a 4D vector
     * @param A Input vector
     * @return A vector containing the ceiling of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Ceil(PA_Vector4_F32 A) noexcept
    {
        return {
            Ceil(A.X),
            Ceil(A.Y),
            Ceil(A.Z),
            Ceil(A.W)
        };
    }
    
    /**
     * @brief Computes the ceiling of each component of a 2x2 matrix
     * @param A Input matrix
     * @return A matrix containing the ceiling of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Ceil(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Ceil(A.A),
            Ceil(A.B)
        };
    }
    
    /**
     * @brief Computes the ceiling of each component of a 3x3 matrix
     * @param A Input matrix
     * @return A matrix containing the ceiling of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Ceil(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Ceil(A.A),
            Ceil(A.B),
            Ceil(A.C)
        };
    }
    
    /**
     * @brief Computes the ceiling of each component of a 4x4 matrix
     * @param A Input matrix
     * @return A matrix containing the ceiling of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Ceil(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Ceil(A.A),
            Ceil(A.B),
            Ceil(A.C),
            Ceil(A.D)
        };
    }
    
    /**
     * @brief Computes the ceiling of each component of a quaternion
     * @param A Input quaternion
     * @return A quaternion containing the ceiling of each component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Ceil(PA_Quaternion_F32 A) noexcept
    {
        return {
            Ceil(A.X),
            Ceil(A.Y),
            Ceil(A.Z),
            Ceil(A.W)
        };
    }
}