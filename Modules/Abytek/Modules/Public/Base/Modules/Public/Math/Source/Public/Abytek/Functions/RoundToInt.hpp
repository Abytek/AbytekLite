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
     * @param A Input 2D vector of floats
     * @return F_Vector2_I32 Vector of 32-bit integers with rounded components
     */
    ABYTEK_FORCE_INLINE F_Vector2_I32 RoundToInt(PA_Vector2_F32 A) noexcept
    {
        return {
            RoundToInt(A.X),
            RoundToInt(A.Y)
        };
    }
    
    /**
     * @brief Rounds each component of a 3D vector to the nearest integer
     * 
     * @param A Input 3D vector of floats
     * @return F_Vector3_I32 Vector of 32-bit integers with rounded components
     */
    ABYTEK_FORCE_INLINE F_Vector3_I32 RoundToInt(PA_Vector3_F32 A) noexcept
    {
        return {
            RoundToInt(A.X),
            RoundToInt(A.Y),
            RoundToInt(A.Z)
        };
    }
    
    /**
     * @brief Rounds each component of a 4D vector to the nearest integer
     * 
     * @param A Input 4D vector of floats
     * @return F_Vector4_I32 Vector of 32-bit integers with rounded components
     */
    ABYTEK_FORCE_INLINE F_Vector4_I32 RoundToInt(PA_Vector4_F32 A) noexcept
    {
        return {
            RoundToInt(A.X),
            RoundToInt(A.Y),
            RoundToInt(A.Z),
            RoundToInt(A.W)
        };
    }
    
    /**
     * @brief Rounds each component of a 2x2 matrix to the nearest integer
     * 
     * @param A Input 2x2 matrix of floats
     * @return F_Matrix2x2_I32 Matrix of 32-bit integers with rounded components
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I32 RoundToInt(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            RoundToInt(A.A),
            RoundToInt(A.B)
        };
    }
    
    /**
     * @brief Rounds each component of a 3x3 matrix to the nearest integer
     * 
     * @param A Input 3x3 matrix of floats
     * @return F_Matrix3x3_I32 Matrix of 32-bit integers with rounded components
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I32 RoundToInt(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            RoundToInt(A.A),
            RoundToInt(A.B),
            RoundToInt(A.C)
        };
    }
    
    /**
     * @brief Rounds each component of a 4x4 matrix to the nearest integer
     * 
     * @param A Input 4x4 matrix of floats
     * @return F_Matrix4x4_I32 Matrix of 32-bit integers with rounded components
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I32 RoundToInt(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            RoundToInt(A.A),
            RoundToInt(A.B),
            RoundToInt(A.C),
            RoundToInt(A.D)
        };
    }
    
    /**
     * @brief Rounds each component of a quaternion to the nearest integer
     * 
     * @param A Input quaternion of floats
     * @return F_Vector4_I32 Vector of 32-bit integers with rounded components
     */
    ABYTEK_FORCE_INLINE F_Vector4_I32 RoundToInt(PA_Quaternion_F32 A) noexcept
    {
        return {
            RoundToInt(A.X),
            RoundToInt(A.Y),
            RoundToInt(A.Z),
            RoundToInt(A.W)
        };
    }
}