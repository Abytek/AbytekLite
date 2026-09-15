#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the sine of each component in a 2D vector
     * 
     * @param A Vector of angles in radians
     * @return F_Vector2_F32 Vector with sine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Sin(PA_Vector2_F32 A) noexcept
    {
        return {
            Sin(A.X),
            Sin(A.Y)
        };
    }
    
    /**
     * @brief Calculates the sine of each component in a 3D vector
     * 
     * @param A Vector of angles in radians
     * @return F_Vector3_F32 Vector with sine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Sin(PA_Vector3_F32 A) noexcept
    {
        return {
            Sin(A.X),
            Sin(A.Y),
            Sin(A.Z)
        };
    }
    
    /**
     * @brief Calculates the sine of each component in a 4D vector
     * 
     * @param A Vector of angles in radians
     * @return F_Vector4_F32 Vector with sine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Sin(PA_Vector4_F32 A) noexcept
    {
        return {
            Sin(A.X),
            Sin(A.Y),
            Sin(A.Z),
            Sin(A.W)
        };
    }
    
    /**
     * @brief Calculates the sine of each component in a 2x2 matrix
     * 
     * @param A Matrix of angles in radians
     * @return F_Matrix2x2_F32 Matrix with sine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Sin(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Sin(A.A),
            Sin(A.B)
        };
    }
    
    /**
     * @brief Calculates the sine of each component in a 3x3 matrix
     * 
     * @param A Matrix of angles in radians
     * @return F_Matrix3x3_F32 Matrix with sine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Sin(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Sin(A.A),
            Sin(A.B),
            Sin(A.C)
        };
    }
    
    /**
     * @brief Calculates the sine of each component in a 4x4 matrix
     * 
     * @param A Matrix of angles in radians
     * @return F_Matrix4x4_F32 Matrix with sine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Sin(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Sin(A.A),
            Sin(A.B),
            Sin(A.C),
            Sin(A.D)
        };
    }
    
    /**
     * @brief Calculates the sine of each component in a quaternion
     * 
     * @param A Quaternion of angles in radians
     * @return F_Quaternion_F32 Quaternion with sine of each component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Sin(PA_Quaternion_F32 A) noexcept
    {
        return {
            Sin(A.X),
            Sin(A.Y),
            Sin(A.Z),
            Sin(A.W)
        };
    }
}