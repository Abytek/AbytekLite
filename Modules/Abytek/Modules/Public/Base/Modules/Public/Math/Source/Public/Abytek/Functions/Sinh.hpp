#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the hyperbolic sine of each component in a 2D vector
     * 
     * @param A Input vector
     * @return F_Vector2_F32 Vector with hyperbolic sine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Sinh(PA_Vector2_F32 A) noexcept
    {
        return {
            Sinh(A.X),
            Sinh(A.Y)
        };
    }
    
    /**
     * @brief Calculates the hyperbolic sine of each component in a 3D vector
     * 
     * @param A Input vector
     * @return F_Vector3_F32 Vector with hyperbolic sine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Sinh(PA_Vector3_F32 A) noexcept
    {
        return {
            Sinh(A.X),
            Sinh(A.Y),
            Sinh(A.Z)
        };
    }
    
    /**
     * @brief Calculates the hyperbolic sine of each component in a 4D vector
     * 
     * @param A Input vector
     * @return F_Vector4_F32 Vector with hyperbolic sine of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Sinh(PA_Vector4_F32 A) noexcept
    {
        return {
            Sinh(A.X),
            Sinh(A.Y),
            Sinh(A.Z),
            Sinh(A.W)
        };
    }
    
    /**
     * @brief Calculates the hyperbolic sine of each component in a 2x2 matrix
     * 
     * @param A Input matrix
     * @return F_Matrix2x2_F32 Matrix with hyperbolic sine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Sinh(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Sinh(A.A),
            Sinh(A.B)
        };
    }
    
    /**
     * @brief Calculates the hyperbolic sine of each component in a 3x3 matrix
     * 
     * @param A Input matrix
     * @return F_Matrix3x3_F32 Matrix with hyperbolic sine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Sinh(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Sinh(A.A),
            Sinh(A.B),
            Sinh(A.C)
        };
    }
    
    /**
     * @brief Calculates the hyperbolic sine of each component in a 4x4 matrix
     * 
     * @param A Input matrix
     * @return F_Matrix4x4_F32 Matrix with hyperbolic sine of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Sinh(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Sinh(A.A),
            Sinh(A.B),
            Sinh(A.C),
            Sinh(A.D)
        };
    }
    
    /**
     * @brief Calculates the hyperbolic sine of each component in a quaternion
     * 
     * @param A Input quaternion
     * @return F_Quaternion_F32 Quaternion with hyperbolic sine of each component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Sinh(PA_Quaternion_F32 A) noexcept
    {
        return {
            Sinh(A.X),
            Sinh(A.Y),
            Sinh(A.Z),
            Sinh(A.W)
        };
    }
}