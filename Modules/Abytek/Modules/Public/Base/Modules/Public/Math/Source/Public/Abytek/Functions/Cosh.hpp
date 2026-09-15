#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Computes the hyperbolic cosine of each component of a 2D vector
     * @param A Input vector
     * @return A vector containing the hyperbolic cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Cosh(PA_Vector2_F32 A) noexcept
    {
        return {
            Cosh(A.X),
            Cosh(A.Y)
        };
    }
    
    /**
     * @brief Computes the hyperbolic cosine of each component of a 3D vector
     * @param A Input vector
     * @return A vector containing the hyperbolic cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Cosh(PA_Vector3_F32 A) noexcept
    {
        return {
            Cosh(A.X),
            Cosh(A.Y),
            Cosh(A.Z)
        };
    }
    
    /**
     * @brief Computes the hyperbolic cosine of each component of a 4D vector
     * @param A Input vector
     * @return A vector containing the hyperbolic cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Cosh(PA_Vector4_F32 A) noexcept
    {
        return {
            Cosh(A.X),
            Cosh(A.Y),
            Cosh(A.Z),
            Cosh(A.W)
        };
    }
    
    /**
     * @brief Computes the hyperbolic cosine of each component of a 2x2 matrix
     * @param A Input matrix
     * @return A matrix containing the hyperbolic cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Cosh(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Cosh(A.A),
            Cosh(A.B)
        };
    }
    
    /**
     * @brief Computes the hyperbolic cosine of each component of a 3x3 matrix
     * @param A Input matrix
     * @return A matrix containing the hyperbolic cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Cosh(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Cosh(A.A),
            Cosh(A.B),
            Cosh(A.C)
        };
    }
    
    /**
     * @brief Computes the hyperbolic cosine of each component of a 4x4 matrix
     * @param A Input matrix
     * @return A matrix containing the hyperbolic cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Cosh(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Cosh(A.A),
            Cosh(A.B),
            Cosh(A.C),
            Cosh(A.D)
        };
    }
    
    /**
     * @brief Computes the hyperbolic cosine of each component of a quaternion
     * @param A Input quaternion
     * @return A quaternion containing the hyperbolic cosine of each component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Cosh(PA_Quaternion_F32 A) noexcept
    {
        return {
            Cosh(A.X),
            Cosh(A.Y),
            Cosh(A.Z),
            Cosh(A.W)
        };
    }
}