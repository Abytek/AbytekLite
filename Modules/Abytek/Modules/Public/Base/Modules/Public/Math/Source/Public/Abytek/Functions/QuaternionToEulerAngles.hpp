#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Cos.hpp"
#include "Abytek/Functions/Sin.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    inline F_Vector3_F32 QuaternionToEulerAngles(PA_Quaternion_F32 Quaternion) noexcept
    {
        const F32 W = Quaternion.W;
        const F32 X = Quaternion.X;
        const F32 Y = Quaternion.Y;
        const F32 Z = Quaternion.Z;

        // Roll (X axis)
        const F32 SinRoll =
            F32(2) * (W * X + Y * Z);

        const F32 CosRoll =
            F32(1) - F32(2) * (X * X + Y * Y);

        const F32 Roll =
            std::atan2(SinRoll, CosRoll);

        // Pitch (Y axis)
        F32 SinPitch =
            F32(2) * (W * Y - Z * X);

        // Protect against floating-point drift outside [-1, 1].
        SinPitch =
            std::max(
                F32(-1),
                std::min(F32(1), SinPitch)
            );

        const F32 Pitch =
            std::asin(SinPitch);

        // Yaw (Z axis)
        const F32 SinYaw =
            F32(2) * (W * Z + X * Y);

        const F32 CosYaw =
            F32(1) - F32(2) * (Y * Y + Z * Z);

        const F32 Yaw =
            std::atan2(SinYaw, CosYaw);

        return F_Vector3_F32
        {
            Roll,
            Pitch,
            Yaw
        };
    }
}