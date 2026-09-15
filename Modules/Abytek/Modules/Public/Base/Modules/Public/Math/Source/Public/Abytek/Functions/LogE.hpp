/**
 * @file LogE.hpp
 * @brief Natural logarithm functions for various numeric types
 * 
 * Provides natural logarithm (base-e) functions for scalar values, vectors, matrices, and quaternions.
 * The logarithm is calculated component-wise for compound types.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Calculates component-wise natural logarithm for a 2D vector
     * 
     * @param A Input vector
     * @return Vector with natural logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 LogE(PA_Vector2_F32 A) noexcept
    {
        return {
            LogE(A.X),
            LogE(A.Y)
        };
    }
    
    /**
     * @brief Calculates component-wise natural logarithm for a 3D vector
     * 
     * @param A Input vector
     * @return Vector with natural logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 LogE(PA_Vector3_F32 A) noexcept
    {
        return {
            LogE(A.X),
            LogE(A.Y),
            LogE(A.Z)
        };
    }
    
    /**
     * @brief Calculates component-wise natural logarithm for a 4D vector
     * 
     * @param A Input vector
     * @return Vector with natural logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 LogE(PA_Vector4_F32 A) noexcept
    {
        return {
            LogE(A.X),
            LogE(A.Y),
            LogE(A.Z),
            LogE(A.W)
        };
    }
    
    /**
     * @brief Calculates component-wise natural logarithm for a 2x2 matrix
     * 
     * @param A Input matrix
     * @return Matrix with natural logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 LogE(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            LogE(A.A),
            LogE(A.B)
        };
    }
    
    /**
     * @brief Calculates component-wise natural logarithm for a 3x3 matrix
     * 
     * @param A Input matrix
     * @return Matrix with natural logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 LogE(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            LogE(A.A),
            LogE(A.B),
            LogE(A.C)
        };
    }
    
    /**
     * @brief Calculates component-wise natural logarithm for a 4x4 matrix
     * 
     * @param A Input matrix
     * @return Matrix with natural logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 LogE(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            LogE(A.A),
            LogE(A.B),
            LogE(A.C),
            LogE(A.D)
        };
    }
    
    /**
     * @brief Calculates component-wise natural logarithm for a quaternion
     * 
     * @param A Input quaternion
     * @return Quaternion with natural logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 LogE(PA_Quaternion_F32 A) noexcept
    {
        return {
            LogE(A.X),
            LogE(A.Y),
            LogE(A.Z),
            LogE(A.W)
        };
    }
}