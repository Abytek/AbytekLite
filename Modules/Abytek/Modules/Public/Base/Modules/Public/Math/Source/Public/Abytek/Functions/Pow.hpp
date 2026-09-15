#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * Computes the component-wise power function for 2D vectors.
     * Each component of the result is A's component raised to the power of B's corresponding component.
     * 
     * @param A Base vector
     * @param B Exponent vector
     * @return Vector with component-wise power operation result
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Pow(PA_Vector2_F32 A, PA_Vector2_F32 B) noexcept
    {
        return {
            Pow(A.X, B.X),
            Pow(A.Y, B.Y)
        };
    }
    
    /**
     * Computes the component-wise power function for 3D vectors.
     * Each component of the result is A's component raised to the power of B's corresponding component.
     * 
     * @param A Base vector
     * @param B Exponent vector
     * @return Vector with component-wise power operation result
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Pow(PA_Vector3_F32 A, PA_Vector3_F32 B) noexcept
    {
        return {
            Pow(A.X, B.X),
            Pow(A.Y, B.Y),
            Pow(A.Z, B.Z)
        };
    }
    
    /**
     * Computes the component-wise power function for 4D vectors.
     * Each component of the result is A's component raised to the power of B's corresponding component.
     * 
     * @param A Base vector
     * @param B Exponent vector
     * @return Vector with component-wise power operation result
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Pow(PA_Vector4_F32 A, PA_Vector4_F32 B) noexcept
    {
        return {
            Pow(A.X, B.X),
            Pow(A.Y, B.Y),
            Pow(A.Z, B.Z),
            Pow(A.W, B.W)
        };
    }
    
    /**
     * Computes the component-wise power function for 2x2 matrices.
     * Each component of the result is A's component raised to the power of B's corresponding component.
     * 
     * @param A Base matrix
     * @param B Exponent matrix
     * @return Matrix with component-wise power operation result
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Pow(PA_Matrix2x2_F32 A, PA_Matrix2x2_F32 B) noexcept
    {
        return {
            Pow(A.A, B.A),
            Pow(A.B, B.B)
        };
    }
    
    /**
     * Computes the component-wise power function for 3x3 matrices.
     * Each component of the result is A's component raised to the power of B's corresponding component.
     * 
     * @param A Base matrix
     * @param B Exponent matrix
     * @return Matrix with component-wise power operation result
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Pow(PA_Matrix3x3_F32 A, PA_Matrix3x3_F32 B) noexcept
    {
        return {
            Pow(A.A, B.A),
            Pow(A.B, B.B),
            Pow(A.C, B.C)
        };
    }
    
    /**
     * Computes the component-wise power function for 4x4 matrices.
     * Each component of the result is A's component raised to the power of B's corresponding component.
     * 
     * @param A Base matrix
     * @param B Exponent matrix
     * @return Matrix with component-wise power operation result
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Pow(PA_Matrix4x4_F32 A, PA_Matrix4x4_F32 B) noexcept
    {
        return {
            Pow(A.A, B.A),
            Pow(A.B, B.B),
            Pow(A.C, B.C),
            Pow(A.D, B.D)
        };
    }
    
    /**
     * Computes the component-wise power function for quaternions.
     * Each component of the result is A's component raised to the power of B's corresponding component.
     * 
     * @param A Base quaternion
     * @param B Exponent quaternion
     * @return Quaternion with component-wise power operation result
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Pow(PA_Quaternion_F32 A, PA_Quaternion_F32 B) noexcept
    {
        return {
            Pow(A.X, B.X),
            Pow(A.Y, B.Y),
            Pow(A.Z, B.Z),
            Pow(A.W, B.W)
        };
    }
}