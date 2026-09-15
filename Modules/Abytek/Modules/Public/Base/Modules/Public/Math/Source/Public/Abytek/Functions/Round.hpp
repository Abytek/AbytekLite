#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Rounds each component of a 2D vector to the nearest integer
     * 
     * @param A Input 2D vector
     * @return F_Vector2_F32 Vector with rounded components
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Round(PA_Vector2_F32 A) noexcept
    {
        return {
            Round(A.X),
            Round(A.Y)
        };
    }
    
    /**
     * @brief Rounds each component of a 3D vector to the nearest integer
     * 
     * @param A Input 3D vector
     * @return F_Vector3_F32 Vector with rounded components
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Round(PA_Vector3_F32 A) noexcept
    {
        return {
            Round(A.X),
            Round(A.Y),
            Round(A.Z)
        };
    }
    
    /**
     * @brief Rounds each component of a 4D vector to the nearest integer
     * 
     * @param A Input 4D vector
     * @return F_Vector4_F32 Vector with rounded components
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Round(PA_Vector4_F32 A) noexcept
    {
        return {
            Round(A.X),
            Round(A.Y),
            Round(A.Z),
            Round(A.W)
        };
    }
    
    /**
     * @brief Rounds each component of a 2x2 matrix to the nearest integer
     * 
     * @param A Input 2x2 matrix
     * @return F_Matrix2x2_F32 Matrix with rounded components
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Round(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Round(A.A),
            Round(A.B)
        };
    }
    
    /**
     * @brief Rounds each component of a 3x3 matrix to the nearest integer
     * 
     * @param A Input 3x3 matrix
     * @return F_Matrix3x3_F32 Matrix with rounded components
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Round(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Round(A.A),
            Round(A.B),
            Round(A.C)
        };
    }
    
    /**
     * @brief Rounds each component of a 4x4 matrix to the nearest integer
     * 
     * @param A Input 4x4 matrix
     * @return F_Matrix4x4_F32 Matrix with rounded components
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Round(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Round(A.A),
            Round(A.B),
            Round(A.C),
            Round(A.D)
        };
    }
    
    /**
     * @brief Rounds each component of a quaternion to the nearest integer
     * 
     * @param A Input quaternion
     * @return F_Quaternion_F32 Quaternion with rounded components
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Round(PA_Quaternion_F32 A) noexcept
    {
        return {
            Round(A.X),
            Round(A.Y),
            Round(A.Z),
            Round(A.W)
        };
    }
}