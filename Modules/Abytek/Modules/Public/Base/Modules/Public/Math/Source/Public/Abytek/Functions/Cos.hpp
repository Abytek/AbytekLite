#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Computes the cosine of each component of a 2D vector
     * @param A Vector of angles in radians
     * @return A vector containing the cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Cos(PA_Vector2_F32 A) noexcept
    {
        return {
            Cos(A.X),
            Cos(A.Y)
        };
    }
    
    /**
     * @brief Computes the cosine of each component of a 3D vector
     * @param A Vector of angles in radians
     * @return A vector containing the cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Cos(PA_Vector3_F32 A) noexcept
    {
        return {
            Cos(A.X),
            Cos(A.Y),
            Cos(A.Z)
        };
    }
    
    /**
     * @brief Computes the cosine of each component of a 4D vector
     * @param A Vector of angles in radians
     * @return A vector containing the cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Cos(PA_Vector4_F32 A) noexcept
    {
        return {
            Cos(A.X),
            Cos(A.Y),
            Cos(A.Z),
            Cos(A.W)
        };
    }
    
    /**
     * @brief Computes the cosine of each component of a 2x2 matrix
     * @param A Matrix of angles in radians
     * @return A matrix containing the cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Cos(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Cos(A.A),
            Cos(A.B)
        };
    }
    
    /**
     * @brief Computes the cosine of each component of a 3x3 matrix
     * @param A Matrix of angles in radians
     * @return A matrix containing the cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Cos(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Cos(A.A),
            Cos(A.B),
            Cos(A.C)
        };
    }
    
    /**
     * @brief Computes the cosine of each component of a 4x4 matrix
     * @param A Matrix of angles in radians
     * @return A matrix containing the cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Cos(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Cos(A.A),
            Cos(A.B),
            Cos(A.C),
            Cos(A.D)
        };
    }
    
    /**
     * @brief Computes the cosine of each component of a quaternion
     * @param A Quaternion of angles in radians
     * @return A quaternion containing the cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Cos(PA_Quaternion_F32 A) noexcept
    {
        return {
            Cos(A.X),
            Cos(A.Y),
            Cos(A.Z),
            Cos(A.W)
        };
    }
}