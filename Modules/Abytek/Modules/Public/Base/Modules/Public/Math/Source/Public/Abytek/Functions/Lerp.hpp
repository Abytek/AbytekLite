/**
 * @file Lerp.hpp
 * @brief Linear interpolation functions for various numeric types
 * 
 * Provides linear interpolation (lerp) functions for scalar values, vectors,
 * matrices, and quaternions. Linear interpolation calculates a value between
 * two endpoints based on an interpolation factor.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Component-wise linear interpolation between two 2D vectors
     * 
     * @param A Start vector when T = 0
     * @param B End vector when T = 1
     * @param T Vector of interpolation factors for each component
     * @return Vector with component-wise interpolated values
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Lerp(PA_Vector2_F32 A, PA_Vector2_F32 B, PA_Vector2_F32 T) noexcept
    {
        return {
            Lerp(A.X, B.X, T.X),
            Lerp(A.Y, B.Y, T.Y)
        };
    }
    
    /**
     * @brief Component-wise linear interpolation between two 3D vectors
     * 
     * @param A Start vector when T = 0
     * @param B End vector when T = 1
     * @param T Vector of interpolation factors for each component
     * @return Vector with component-wise interpolated values
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Lerp(PA_Vector3_F32 A, PA_Vector3_F32 B, PA_Vector3_F32 T) noexcept
    {
        return {
            Lerp(A.X, B.X, T.X),
            Lerp(A.Y, B.Y, T.Y),
            Lerp(A.Z, B.Z, T.Z)
        };
    }
    
    /**
     * @brief Component-wise linear interpolation between two 4D vectors
     * 
     * @param A Start vector when T = 0
     * @param B End vector when T = 1
     * @param T Vector of interpolation factors for each component
     * @return Vector with component-wise interpolated values
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Lerp(PA_Vector4_F32 A, PA_Vector4_F32 B, PA_Vector4_F32 T) noexcept
    {
        return {
            Lerp(A.X, B.X, T.X),
            Lerp(A.Y, B.Y, T.Y),
            Lerp(A.Z, B.Z, T.Z),
            Lerp(A.W, B.W, T.W)
        };
    }
    
    /**
     * @brief Component-wise linear interpolation between two 2x2 matrices
     * 
     * @param A Start matrix when T = 0
     * @param B End matrix when T = 1
     * @param T Matrix of interpolation factors for each component
     * @return Matrix with component-wise interpolated values
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Lerp(PA_Matrix2x2_F32 A, PA_Matrix2x2_F32 B, PA_Matrix2x2_F32 T) noexcept
    {
        return {
            Lerp(A.A, B.A, T.A),
            Lerp(A.B, B.B, T.B)
        };
    }
    
    /**
     * @brief Component-wise linear interpolation between two 3x3 matrices
     * 
     * @param A Start matrix when T = 0
     * @param B End matrix when T = 1
     * @param T Matrix of interpolation factors for each component
     * @return Matrix with component-wise interpolated values
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Lerp(PA_Matrix3x3_F32 A, PA_Matrix3x3_F32 B, PA_Matrix3x3_F32 T) noexcept
    {
        return {
            Lerp(A.A, B.A, T.A),
            Lerp(A.B, B.B, T.B),
            Lerp(A.C, B.C, T.C)
        };
    }
    
    /**
     * @brief Component-wise linear interpolation between two 4x4 matrices
     * 
     * @param A Start matrix when T = 0
     * @param B End matrix when T = 1
     * @param T Matrix of interpolation factors for each component
     * @return Matrix with component-wise interpolated values
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Lerp(PA_Matrix4x4_F32 A, PA_Matrix4x4_F32 B, PA_Matrix4x4_F32 T) noexcept
    {
        return {
            Lerp(A.A, B.A, T.A),
            Lerp(A.B, B.B, T.B),
            Lerp(A.C, B.C, T.C),
            Lerp(A.D, B.D, T.D)
        };
    }
    
    /**
     * @brief Component-wise linear interpolation between two quaternions
     * 
     * Note: For proper quaternion interpolation, Slerp should typically be used instead.
     * This function performs a simple linear interpolation on each component.
     * 
     * @param A Start quaternion when T = 0
     * @param B End quaternion when T = 1
     * @param T Quaternion of interpolation factors for each component
     * @return Quaternion with component-wise interpolated values
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Lerp(PA_Quaternion_F32 A, PA_Quaternion_F32 B, PA_Quaternion_F32 T) noexcept
    {
        return {
            Lerp(A.X, B.X, T.X),
            Lerp(A.Y, B.Y, T.Y),
            Lerp(A.Z, B.Z, T.Z),
            Lerp(A.W, B.W, T.W)
        };
    }
}