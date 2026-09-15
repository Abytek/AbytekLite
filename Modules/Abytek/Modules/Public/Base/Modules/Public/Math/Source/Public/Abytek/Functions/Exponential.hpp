/**
 * @file Exponential.hpp
 * @brief Exponential functions for various numeric types
 * 
 * Provides exponential functions (e^x) for scalar values, vectors, matrices, and quaternions.
 * The exponential is calculated component-wise for compound types.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Calculates component-wise exponential for a 2D vector
     * 
     * @param A Input vector
     * @return Vector with exponential applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Exponential(PA_Vector2_F32 A) noexcept
    {
        return {
            Exponential(A.X),
            Exponential(A.Y)
        };
    }
    
    /**
     * @brief Calculates component-wise exponential for a 3D vector
     * 
     * @param A Input vector
     * @return Vector with exponential applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Exponential(PA_Vector3_F32 A) noexcept
    {
        return {
            Exponential(A.X),
            Exponential(A.Y),
            Exponential(A.Z)
        };
    }
    
    /**
     * @brief Calculates component-wise exponential for a 4D vector
     * 
     * @param A Input vector
     * @return Vector with exponential applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Exponential(PA_Vector4_F32 A) noexcept
    {
        return {
            Exponential(A.X),
            Exponential(A.Y),
            Exponential(A.Z),
            Exponential(A.W)
        };
    }
    
    /**
     * @brief Calculates component-wise exponential for a 2x2 matrix
     * 
     * @param A Input matrix
     * @return Matrix with exponential applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Exponential(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Exponential(A.A),
            Exponential(A.B)
        };
    }
    
    /**
     * @brief Calculates component-wise exponential for a 3x3 matrix
     * 
     * @param A Input matrix
     * @return Matrix with exponential applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Exponential(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Exponential(A.A),
            Exponential(A.B),
            Exponential(A.C)
        };
    }
    
    /**
     * @brief Calculates component-wise exponential for a 4x4 matrix
     * 
     * @param A Input matrix
     * @return Matrix with exponential applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Exponential(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Exponential(A.A),
            Exponential(A.B),
            Exponential(A.C),
            Exponential(A.D)
        };
    }
    
    /**
     * @brief Calculates component-wise exponential for a quaternion
     * 
     * Note: This is not the quaternion exponential in the mathematical sense,
     * but rather applies the exponential function to each component.
     * 
     * @param A Input quaternion
     * @return Quaternion with exponential applied to each component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Exponential(PA_Quaternion_F32 A) noexcept
    {
        return {
            Exponential(A.X),
            Exponential(A.Y),
            Exponential(A.Z),
            Exponential(A.W)
        };
    }
}