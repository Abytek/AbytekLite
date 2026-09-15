/**
 * @file Log2.hpp
 * @brief Base-2 logarithm functions for various numeric types
 * 
 * Provides base-2 logarithm functions for scalar values, vectors, matrices, and quaternions.
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
     * @brief Calculates component-wise base-2 logarithm for a 2D vector
     * 
     * @param A Input vector
     * @return Vector with base-2 logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Log2(PA_Vector2_F32 A) noexcept
    {
        return {
            Log2(A.X),
            Log2(A.Y)
        };
    }
    
    /**
     * @brief Calculates component-wise base-2 logarithm for a 3D vector
     * 
     * @param A Input vector
     * @return Vector with base-2 logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Log2(PA_Vector3_F32 A) noexcept
    {
        return {
            Log2(A.X),
            Log2(A.Y),
            Log2(A.Z)
        };
    }
    
    /**
     * @brief Calculates component-wise base-2 logarithm for a 4D vector
     * 
     * @param A Input vector
     * @return Vector with base-2 logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Log2(PA_Vector4_F32 A) noexcept
    {
        return {
            Log2(A.X),
            Log2(A.Y),
            Log2(A.Z),
            Log2(A.W)
        };
    }
    
    /**
     * @brief Calculates component-wise base-2 logarithm for a 2x2 matrix
     * 
     * @param A Input matrix
     * @return Matrix with base-2 logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Log2(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Log2(A.A),
            Log2(A.B)
        };
    }
    
    /**
     * @brief Calculates component-wise base-2 logarithm for a 3x3 matrix
     * 
     * @param A Input matrix
     * @return Matrix with base-2 logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Log2(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Log2(A.A),
            Log2(A.B),
            Log2(A.C)
        };
    }
    
    /**
     * @brief Calculates component-wise base-2 logarithm for a 4x4 matrix
     * 
     * @param A Input matrix
     * @return Matrix with base-2 logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Log2(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Log2(A.A),
            Log2(A.B),
            Log2(A.C),
            Log2(A.D)
        };
    }
    
    /**
     * @brief Calculates component-wise base-2 logarithm for a quaternion
     * 
     * @param A Input quaternion
     * @return Quaternion with base-2 logarithm applied to each component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Log2(PA_Quaternion_F32 A) noexcept
    {
        return {
            Log2(A.X),
            Log2(A.Y),
            Log2(A.Z),
            Log2(A.W)
        };
    }
}