#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/MakePosition4.hpp"


namespace Abytek
{
    /**
     * Creates a 4x4 translation matrix from a 3D vector.
     * 
     * @param Translation Vector containing translation values for each axis (X, Y, Z)
     * @return 4x4 matrix with translation values in the fourth column
     */
    inline F_Matrix4x4_F32 MakeTranslationMatrix(PA_Vector3_F32 Translation) noexcept
    {
        return {
            F_Vector4_F32::Right(),
            F_Vector4_F32::Up(),
            F_Vector4_F32::Forward(),
            MakePosition4(Translation)
        };
    }
}