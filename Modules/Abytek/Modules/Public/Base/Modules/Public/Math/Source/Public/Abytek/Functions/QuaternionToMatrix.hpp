#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Operators/QuaternionMatrix.hpp"


namespace Abytek
{
    /**
     * @brief Converts a quaternion to a 3x3 linear transformation matrix
     * 
     * Creates a rotation matrix representation from the input quaternion.
     * This is useful for applying the quaternion rotation to vectors.
     * 
     * @param Q Input quaternion to convert
     * @return F_Matrix3x3_F32 3x3 rotation matrix equivalent to the quaternion rotation
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 QuaternionToLinearMatrix(PA_Quaternion_F32 Q) noexcept
    {
        return Q * Identity<F_Matrix3x3_F32>();
    }
    
    /**
     * @brief Converts a quaternion to a 4x4 affine transformation matrix
     * 
     * Creates a rotation matrix representation from the input quaternion.
     * The resulting 4x4 matrix includes identity translation and scale components,
     * making it suitable for use in 3D transformation pipelines.
     * 
     * @param Q Input quaternion to convert
     * @return F_Matrix4x4_F32 4x4 affine matrix equivalent to the quaternion rotation
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 QuaternionToAffineMatrix(PA_Quaternion_F32 Q) noexcept
    {
        return Q * Identity<F_Matrix4x4_F32>();
    }
}