/**
 * @file Floor.hpp
 * @brief Floor functions for various numeric types
 * 
 * Provides floor functions (round down to nearest integer) for scalar values,
 * vectors, matrices, and quaternions. The floor operation is calculated
 * component-wise for compound types.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Calculates component-wise floor for a 2D vector
     * 
     * @param A Input vector
     * @return Vector with floor applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Floor(PA_Vector2_F32 A) noexcept
    {
        return {
            Floor(A.X),
            Floor(A.Y)
        };
    }
    
    /**
     * @brief Calculates component-wise floor for a 3D vector
     * 
     * @param A Input vector
     * @return Vector with floor applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Floor(PA_Vector3_F32 A) noexcept
    {
        return {
            Floor(A.X),
            Floor(A.Y),
            Floor(A.Z)
        };
    }
    
    /**
     * @brief Calculates component-wise floor for a 4D vector
     * 
     * @param A Input vector
     * @return Vector with floor applied to each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Floor(PA_Vector4_F32 A) noexcept
    {
        return {
            Floor(A.X),
            Floor(A.Y),
            Floor(A.Z),
            Floor(A.W)
        };
    }
    
    /**
     * @brief Calculates component-wise floor for a 2x2 matrix
     * 
     * @param A Input matrix
     * @return Matrix with floor applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Floor(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Floor(A.A),
            Floor(A.B)
        };
    }
    
    /**
     * @brief Calculates component-wise floor for a 3x3 matrix
     * 
     * @param A Input matrix
     * @return Matrix with floor applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Floor(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Floor(A.A),
            Floor(A.B),
            Floor(A.C)
        };
    }
    
    /**
     * @brief Calculates component-wise floor for a 4x4 matrix
     * 
     * @param A Input matrix
     * @return Matrix with floor applied to each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Floor(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Floor(A.A),
            Floor(A.B),
            Floor(A.C),
            Floor(A.D)
        };
    }
    
    /**
     * @brief Calculates component-wise floor for a quaternion
     * 
     * @param A Input quaternion
     * @return Quaternion with floor applied to each component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Floor(PA_Quaternion_F32 A) noexcept
    {
        return {
            Floor(A.X),
            Floor(A.Y),
            Floor(A.Z),
            Floor(A.W)
        };
    }
}