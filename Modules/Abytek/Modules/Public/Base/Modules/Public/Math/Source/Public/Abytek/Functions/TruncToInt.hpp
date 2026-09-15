/**
 * @file TruncToInt.hpp
 * @brief Functions for truncating floating point values to integers.
 * 
 * Provides optimized, inlined truncation functions for converting various float types to 
 * their integer equivalents by discarding the decimal portion.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Truncates a 2D float vector to a 2D integer vector.
     * @param A The floating point vector to truncate.
     * @return A vector containing the truncated integer values.
     */
    ABYTEK_FORCE_INLINE F_Vector2_I32 TruncToInt(PA_Vector2_F32 A) noexcept
    {
        return {
            TruncToInt(A.X),
            TruncToInt(A.Y)
        };
    }
    
    /**
     * @brief Truncates a 3D float vector to a 3D integer vector.
     * @param A The floating point vector to truncate.
     * @return A vector containing the truncated integer values.
     */
    ABYTEK_FORCE_INLINE F_Vector3_I32 TruncToInt(PA_Vector3_F32 A) noexcept
    {
        return {
            TruncToInt(A.X),
            TruncToInt(A.Y),
            TruncToInt(A.Z)
        };
    }
    
    /**
     * @brief Truncates a 4D float vector to a 4D integer vector.
     * @param A The floating point vector to truncate.
     * @return A vector containing the truncated integer values.
     */
    ABYTEK_FORCE_INLINE F_Vector4_I32 TruncToInt(PA_Vector4_F32 A) noexcept
    {
        return {
            TruncToInt(A.X),
            TruncToInt(A.Y),
            TruncToInt(A.Z),
            TruncToInt(A.W)
        };
    }
    
    /**
     * @brief Truncates a 2x2 float matrix to a 2x2 integer matrix.
     * @param A The floating point matrix to truncate.
     * @return A matrix containing the truncated integer values.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I32 TruncToInt(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            TruncToInt(A.A),
            TruncToInt(A.B)
        };
    }
    
    /**
     * @brief Truncates a 3x3 float matrix to a 3x3 integer matrix.
     * @param A The floating point matrix to truncate.
     * @return A matrix containing the truncated integer values.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I32 TruncToInt(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            TruncToInt(A.A),
            TruncToInt(A.B),
            TruncToInt(A.C)
        };
    }
    
    /**
     * @brief Truncates a 4x4 float matrix to a 4x4 integer matrix.
     * @param A The floating point matrix to truncate.
     * @return A matrix containing the truncated integer values.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I32 TruncToInt(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            TruncToInt(A.A),
            TruncToInt(A.B),
            TruncToInt(A.C),
            TruncToInt(A.D)
        };
    }
    
    /**
     * @brief Truncates a quaternion to a 4D integer vector.
     * @param A The floating point quaternion to truncate.
     * @return A vector containing the truncated integer values.
     */
    ABYTEK_FORCE_INLINE F_Vector4_I32 TruncToInt(PA_Quaternion_F32 A) noexcept
    {
        return {
            TruncToInt(A.X),
            TruncToInt(A.Y),
            TruncToInt(A.Z),
            TruncToInt(A.W)
        };
    }
}