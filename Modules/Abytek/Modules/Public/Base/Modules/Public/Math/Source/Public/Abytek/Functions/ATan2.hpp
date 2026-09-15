#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Computes the 2-argument arctangent (atan2) component-wise for two 2D vectors
     * @param A Vector containing Y values (numerators)
     * @param B Vector containing X values (denominators)
     * @return A vector containing the atan2 of corresponding components
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 ATan2(PA_Vector2_F32 A, PA_Vector2_F32 B) noexcept
    {
        return {
            ATan2(A.X, B.X),
            ATan2(A.Y, B.Y)
        };
    }
    
    /**
     * @brief Computes the 2-argument arctangent (atan2) component-wise for two 3D vectors
     * @param A Vector containing Y values (numerators)
     * @param B Vector containing X values (denominators)
     * @return A vector containing the atan2 of corresponding components
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 ATan2(PA_Vector3_F32 A, PA_Vector3_F32 B) noexcept
    {
        return {
            ATan2(A.X, B.X),
            ATan2(A.Y, B.Y),
            ATan2(A.Z, B.Z)
        };
    }
    
    /**
     * @brief Computes the 2-argument arctangent (atan2) component-wise for two 4D vectors
     * @param A Vector containing Y values (numerators)
     * @param B Vector containing X values (denominators)
     * @return A vector containing the atan2 of corresponding components
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 ATan2(PA_Vector4_F32 A, PA_Vector4_F32 B) noexcept
    {
        return {
            ATan2(A.X, B.X),
            ATan2(A.Y, B.Y),
            ATan2(A.Z, B.Z),
            ATan2(A.W, B.W)
        };
    }
    
    /**
     * @brief Computes the 2-argument arctangent (atan2) component-wise for two 2x2 matrices
     * @param A Matrix containing Y values (numerators)
     * @param B Matrix containing X values (denominators)
     * @return A matrix containing the atan2 of corresponding components
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 ATan2(PA_Matrix2x2_F32 A, PA_Matrix2x2_F32 B) noexcept
    {
        return {
            ATan2(A.A, B.A),
            ATan2(A.B, B.B)
        };
    }
    
    /**
     * @brief Computes the 2-argument arctangent (atan2) component-wise for two 3x3 matrices
     * @param A Matrix containing Y values (numerators)
     * @param B Matrix containing X values (denominators)
     * @return A matrix containing the atan2 of corresponding components
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 ATan2(PA_Matrix3x3_F32 A, PA_Matrix3x3_F32 B) noexcept
    {
        return {
            ATan2(A.A, B.A),
            ATan2(A.B, B.B),
            ATan2(A.C, B.C)
        };
    }
    
    /**
     * @brief Computes the 2-argument arctangent (atan2) component-wise for two 4x4 matrices
     * @param A Matrix containing Y values (numerators)
     * @param B Matrix containing X values (denominators)
     * @return A matrix containing the atan2 of corresponding components
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 ATan2(PA_Matrix4x4_F32 A, PA_Matrix4x4_F32 B) noexcept
    {
        return {
            ATan2(A.A, B.A),
            ATan2(A.B, B.B),
            ATan2(A.C, B.C),
            ATan2(A.D, B.D)
        };
    }
    
    /**
     * @brief Computes the 2-argument arctangent (atan2) component-wise for two quaternions
     * @param A Quaternion containing Y values (numerators)
     * @param B Quaternion containing X values (denominators)
     * @return A quaternion containing the atan2 of corresponding components
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 ATan2(PA_Quaternion_F32 A, PA_Quaternion_F32 B) noexcept
    {
        return {
            ATan2(A.X, B.X),
            ATan2(A.Y, B.Y),
            ATan2(A.Z, B.Z),
            ATan2(A.W, B.W)
        };
    }
}