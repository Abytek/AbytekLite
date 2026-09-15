#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/MakePosition4.hpp"


namespace Abytek
{
    /**
     * Creates a 3x3 scale matrix from a 3D vector.
     * 
     * @param Scale Vector containing scale factors for each axis (X, Y, Z)
     * @return 3x3 matrix with scale factors applied along the diagonal
     */
    inline F_Matrix3x3_F32 MakeScaleMatrix(PA_Vector3_F32 Scale) noexcept
    {
        return {
            F_Vector3_F32::Right() * Scale.X,
            F_Vector3_F32::Up() * Scale.Y,
            F_Vector3_F32::Forward() * Scale.Z
        };
    }
    
    inline F_Matrix4x4_F32 MakeScaleMatrix4x4(PA_Vector3_F32 Scale) noexcept
    {
        return {
            F_Vector4_F32::Right() * Scale.X,
            F_Vector4_F32::Up() * Scale.Y,
            F_Vector4_F32::Forward() * Scale.Z,
            F_Vector4_F32::Future()
        };
    }
}