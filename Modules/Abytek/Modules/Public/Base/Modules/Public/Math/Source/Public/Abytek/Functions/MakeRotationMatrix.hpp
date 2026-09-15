#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/MakePosition4.hpp"
#include "Abytek/Functions/Cos.hpp"
#include "Abytek/Functions/Sin.hpp"

namespace Abytek
{
    /**
     * @brief Creates a 3x3 left-handed rotation matrix from Euler angles.
     *
     * Coordinate system:
     *   +X = Right
     *   +Y = Up
     *   +Z = Forward
     *
     * Euler angles:
     *   X = Pitch
     *   Y = Yaw
     *   Z = Roll
     *
     * Rotation order: Z-Y-X.
     *
     * @param EulerAngles Vector3 containing rotation angles in radians.
     * @return A 3x3 left-handed rotation matrix.
     */
    inline F_Matrix3x3_F32 MakeRotationMatrix(PA_Vector3_F32 EulerAngles) noexcept
    {
        const F32 sx = Sin(EulerAngles.X);
        const F32 cx = Cos(EulerAngles.X);

        const F32 sy = Sin(EulerAngles.Y);
        const F32 cy = Cos(EulerAngles.Y);

        const F32 sz = Sin(EulerAngles.Z);
        const F32 cz = Cos(EulerAngles.Z);

        return {
            F_Vector3_F32 {
                cy * cz,
                sx * sy * cz + cx * sz,
                -cx * sy * cz + sx * sz
            },
            F_Vector3_F32 {
                -cy * sz,
                -sx * sy * sz + cx * cz,
                cx * sy * sz + sx * cz
            },
            F_Vector3_F32 {
                sy,
                -sx * cy,
                cx * cy
            }
        };
    }

    inline F_Matrix4x4_F32 MakeRotationMatrix4x4(PA_Vector3_F32 EulerAngles) noexcept
    {
        const F32 sx = Sin(EulerAngles.X);
        const F32 cx = Cos(EulerAngles.X);

        const F32 sy = Sin(EulerAngles.Y);
        const F32 cy = Cos(EulerAngles.Y);

        const F32 sz = Sin(EulerAngles.Z);
        const F32 cz = Cos(EulerAngles.Z);

        return {
            F_Vector4_F32 {
                cy * cz,
                sx * sy * cz + cx * sz,
                -cx * sy * cz + sx * sz,
                0.0f
            },
            F_Vector4_F32 {
                -cy * sz,
                -sx * sy * sz + cx * cz,
                cx * sy * sz + sx * cz,
                0.0f
            },
            F_Vector4_F32 {
                sy,
                -sx * cy,
                cx * cy,
                0.0f
            },
            F_Vector4_F32::Future()
        };
    }

    /**
     * @brief Creates a left-handed rotation matrix around the X axis.
     *
     * +X = Right
     * +Y = Up
     * +Z = Forward
     */
    inline F_Matrix3x3_F32 MakeRotationMatrixX(F32 X) noexcept
    {
        const F32 s = Sin(X);
        const F32 c = Cos(X);

        return {
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, c, -s },
            { 0.0f, s,  c }
        };
    }

    /**
     * @brief Creates a left-handed rotation matrix around the Y axis.
     */
    inline F_Matrix3x3_F32 MakeRotationMatrixY(F32 Y) noexcept
    {
        const F32 s = Sin(Y);
        const F32 c = Cos(Y);

        return {
            {  c, 0.0f, s },
            { 0.0f, 1.0f, 0.0f },
            { -s, 0.0f, c }
        };
    }

    /**
     * @brief Creates a left-handed rotation matrix around the Z axis.
     */
    inline F_Matrix3x3_F32 MakeRotationMatrixZ(F32 Z) noexcept
    {
        const F32 s = Sin(Z);
        const F32 c = Cos(Z);

        return {
            { c, -s, 0.0f },
            { s,  c, 0.0f },
            { 0.0f, 0.0f, 1.0f }
        };
    }
}