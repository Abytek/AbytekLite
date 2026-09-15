/**
 * @file SquareRoot.hpp
 * @brief Functions for calculating the square root of a value (√A).
 * 
 * Provides optimized, inlined square root functions for various numeric types and vectors.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the component-wise square root of a 2D float vector.
     * @param A The vector to calculate the square root of.
     * @return A vector containing the square root of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 SquareRoot(PA_Vector2_F32 A) noexcept
    {
        return {
            SquareRoot(A.X),
            SquareRoot(A.Y)
        };
    }
    
    /**
     * @brief Calculates the component-wise square root of a 3D float vector.
     * @param A The vector to calculate the square root of.
     * @return A vector containing the square root of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 SquareRoot(PA_Vector3_F32 A) noexcept
    {
        return {
            SquareRoot(A.X),
            SquareRoot(A.Y),
            SquareRoot(A.Z)
        };
    }
    
    /**
     * @brief Calculates the component-wise square root of a 4D float vector.
     * @param A The vector to calculate the square root of.
     * @return A vector containing the square root of each component.
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 SquareRoot(PA_Vector4_F32 A) noexcept
    {
        return {
            SquareRoot(A.X),
            SquareRoot(A.Y),
            SquareRoot(A.Z),
            SquareRoot(A.W)
        };
    }
    
    /**
     * @brief Calculates the component-wise square root of a 2x2 float matrix.
     * @param A The matrix to calculate the square root of.
     * @return A matrix containing the square root of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 SquareRoot(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            SquareRoot(A.A),
            SquareRoot(A.B)
        };
    }
    
    /**
     * @brief Calculates the component-wise square root of a 3x3 float matrix.
     * @param A The matrix to calculate the square root of.
     * @return A matrix containing the square root of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 SquareRoot(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            SquareRoot(A.A),
            SquareRoot(A.B),
            SquareRoot(A.C)
        };
    }
    
    /**
     * @brief Calculates the component-wise square root of a 4x4 float matrix.
     * @param A The matrix to calculate the square root of.
     * @return A matrix containing the square root of each component.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 SquareRoot(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            SquareRoot(A.A),
            SquareRoot(A.B),
            SquareRoot(A.C),
            SquareRoot(A.D)
        };
    }
    
    /**
     * @brief Calculates the component-wise square root of a quaternion.
     * @param A The quaternion to calculate the square root of.
     * @return A quaternion containing the square root of each component.
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 SquareRoot(PA_Quaternion_F32 A) noexcept
    {
        return {
            SquareRoot(A.X),
            SquareRoot(A.Y),
            SquareRoot(A.Z),
            SquareRoot(A.W)
        };
    }
}