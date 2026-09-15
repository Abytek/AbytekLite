#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Saturates each component of a 2D vector to the range [0.0, 1.0]
     * 
     * @param A Input 2D vector
     * @return F_Vector2_F32 Vector with components saturated to [0.0, 1.0]
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Saturate(PA_Vector2_F32 A) noexcept
    {
        return {
            Saturate(A.X),
            Saturate(A.Y)
        };
    }
    
    /**
     * @brief Saturates each component of a 3D vector to the range [0.0, 1.0]
     * 
     * @param A Input 3D vector
     * @return F_Vector3_F32 Vector with components saturated to [0.0, 1.0]
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Saturate(PA_Vector3_F32 A) noexcept
    {
        return {
            Saturate(A.X),
            Saturate(A.Y),
            Saturate(A.Z)
        };
    }
    
    /**
     * @brief Saturates each component of a 4D vector to the range [0.0, 1.0]
     * 
     * @param A Input 4D vector
     * @return F_Vector4_F32 Vector with components saturated to [0.0, 1.0]
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Saturate(PA_Vector4_F32 A) noexcept
    {
        return {
            Saturate(A.X),
            Saturate(A.Y),
            Saturate(A.Z),
            Saturate(A.W)
        };
    }
    
    /**
     * @brief Saturates each component of a 2x2 matrix to the range [0.0, 1.0]
     * 
     * @param A Input 2x2 matrix
     * @return F_Matrix2x2_F32 Matrix with components saturated to [0.0, 1.0]
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Saturate(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Saturate(A.A),
            Saturate(A.B)
        };
    }
    
    /**
     * @brief Saturates each component of a 3x3 matrix to the range [0.0, 1.0]
     * 
     * @param A Input 3x3 matrix
     * @return F_Matrix3x3_F32 Matrix with components saturated to [0.0, 1.0]
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Saturate(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Saturate(A.A),
            Saturate(A.B),
            Saturate(A.C)
        };
    }
    
    /**
     * @brief Saturates each component of a 4x4 matrix to the range [0.0, 1.0]
     * 
     * @param A Input 4x4 matrix
     * @return F_Matrix4x4_F32 Matrix with components saturated to [0.0, 1.0]
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Saturate(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Saturate(A.A),
            Saturate(A.B),
            Saturate(A.C),
            Saturate(A.D)
        };
    }
    
    /**
     * @brief Saturates each component of a quaternion to the range [0.0, 1.0]
     * 
     * @param A Input quaternion
     * @return F_Quaternion_F32 Quaternion with components saturated to [0.0, 1.0]
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Saturate(PA_Quaternion_F32 A) noexcept
    {
        return {
            Saturate(A.X),
            Saturate(A.Y),
            Saturate(A.Z),
            Saturate(A.W)
        };
    }
}