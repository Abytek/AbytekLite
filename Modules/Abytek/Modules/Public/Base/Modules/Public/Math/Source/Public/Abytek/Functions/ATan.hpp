#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Computes the arctangent (inverse tangent) of each component of a 2D vector
     * @param A Input vector
     * @return A vector containing the arctangent of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 ATan(PA_Vector2_F32 A) noexcept
    {
        return {
            ATan(A.X),
            ATan(A.Y)
        };
    }
    
    /**
     * @brief Computes the arctangent (inverse tangent) of each component of a 3D vector
     * @param A Input vector
     * @return A vector containing the arctangent of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 ATan(PA_Vector3_F32 A) noexcept
    {
        return {
            ATan(A.X),
            ATan(A.Y),
            ATan(A.Z)
        };
    }
    
    /**
     * @brief Computes the arctangent (inverse tangent) of each component of a 4D vector
     * @param A Input vector
     * @return A vector containing the arctangent of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 ATan(PA_Vector4_F32 A) noexcept
    {
        return {
            ATan(A.X),
            ATan(A.Y),
            ATan(A.Z),
            ATan(A.W)
        };
    }
    
    /**
     * @brief Computes the arctangent (inverse tangent) of each component of a 2x2 matrix
     * @param A Input matrix
     * @return A matrix containing the arctangent of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 ATan(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            ATan(A.A),
            ATan(A.B)
        };
    }
    
    /**
     * @brief Computes the arctangent (inverse tangent) of each component of a 3x3 matrix
     * @param A Input matrix
     * @return A matrix containing the arctangent of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 ATan(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            ATan(A.A),
            ATan(A.B),
            ATan(A.C)
        };
    }
    
    /**
     * @brief Computes the arctangent (inverse tangent) of each component of a 4x4 matrix
     * @param A Input matrix
     * @return A matrix containing the arctangent of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 ATan(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            ATan(A.A),
            ATan(A.B),
            ATan(A.C),
            ATan(A.D)
        };
    }
    
    /**
     * @brief Computes the arctangent (inverse tangent) of each component of a quaternion
     * @param A Input quaternion
     * @return A quaternion containing the arctangent of each component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 ATan(PA_Quaternion_F32 A) noexcept
    {
        return {
            ATan(A.X),
            ATan(A.Y),
            ATan(A.Z),
            ATan(A.W)
        };
    }
}