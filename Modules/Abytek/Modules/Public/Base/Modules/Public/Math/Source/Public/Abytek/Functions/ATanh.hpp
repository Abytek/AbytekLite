#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Computes the hyperbolic arctangent (inverse hyperbolic tangent) of each component of a 2D vector
     * @param A Input vector with components in the range (-1, 1)
     * @return A vector containing the hyperbolic arctangent of each component
     * @note The domain of ATanh is (-1, 1). Components outside this range will result in NaN or infinity.
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 ATanh(PA_Vector2_F32 A) noexcept
    {
        return {
            ATanh(A.X),
            ATanh(A.Y)
        };
    }
    
    /**
     * @brief Computes the hyperbolic arctangent (inverse hyperbolic tangent) of each component of a 3D vector
     * @param A Input vector with components in the range (-1, 1)
     * @return A vector containing the hyperbolic arctangent of each component
     * @note The domain of ATanh is (-1, 1). Components outside this range will result in NaN or infinity.
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 ATanh(PA_Vector3_F32 A) noexcept
    {
        return {
            ATanh(A.X),
            ATanh(A.Y),
            ATanh(A.Z)
        };
    }
    
    /**
     * @brief Computes the hyperbolic arctangent (inverse hyperbolic tangent) of each component of a 4D vector
     * @param A Input vector with components in the range (-1, 1)
     * @return A vector containing the hyperbolic arctangent of each component
     * @note The domain of ATanh is (-1, 1). Components outside this range will result in NaN or infinity.
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 ATanh(PA_Vector4_F32 A) noexcept
    {
        return {
            ATanh(A.X),
            ATanh(A.Y),
            ATanh(A.Z),
            ATanh(A.W)
        };
    }
    
    /**
     * @brief Computes the hyperbolic arctangent (inverse hyperbolic tangent) of each component of a 2x2 matrix
     * @param A Input matrix with components in the range (-1, 1)
     * @return A matrix containing the hyperbolic arctangent of each component
     * @note The domain of ATanh is (-1, 1). Components outside this range will result in NaN or infinity.
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 ATanh(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            ATanh(A.A),
            ATanh(A.B)
        };
    }
    
    /**
     * @brief Computes the hyperbolic arctangent (inverse hyperbolic tangent) of each component of a 3x3 matrix
     * @param A Input matrix with components in the range (-1, 1)
     * @return A matrix containing the hyperbolic arctangent of each component
     * @note The domain of ATanh is (-1, 1). Components outside this range will result in NaN or infinity.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 ATanh(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            ATanh(A.A),
            ATanh(A.B),
            ATanh(A.C)
        };
    }
    
    /**
     * @brief Computes the hyperbolic arctangent (inverse hyperbolic tangent) of each component of a 4x4 matrix
     * @param A Input matrix with components in the range (-1, 1)
     * @return A matrix containing the hyperbolic arctangent of each component
     * @note The domain of ATanh is (-1, 1). Components outside this range will result in NaN or infinity.
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 ATanh(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            ATanh(A.A),
            ATanh(A.B),
            ATanh(A.C),
            ATanh(A.D)
        };
    }
    
    /**
     * @brief Computes the hyperbolic arctangent (inverse hyperbolic tangent) of each component of a quaternion
     * @param A Input quaternion with components in the range (-1, 1)
     * @return A quaternion containing the hyperbolic arctangent of each component
     * @note The domain of ATanh is (-1, 1). Components outside this range will result in NaN or infinity.
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 ATanh(PA_Quaternion_F32 A) noexcept
    {
        return {
            ATanh(A.X),
            ATanh(A.Y),
            ATanh(A.Z),
            ATanh(A.W)
        };
    }
}