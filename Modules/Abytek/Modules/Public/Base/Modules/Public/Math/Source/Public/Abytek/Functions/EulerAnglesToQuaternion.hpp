/**
 * @file EulerAnglesToQuaternion.hpp
 * @brief Conversion from Euler angles to quaternion representation
 * 
 * Provides functionality to convert from Euler angles (X=pitch, Y=yaw, Z=roll)
 * to quaternion representation for 3D rotations.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Cos.hpp"
#include "Abytek/Functions/Sin.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Converts Euler angles to a quaternion
     * 
     * Converts rotation angles in radians around the X, Y, and Z axes
     * (pitch, yaw, and roll respectively) into a quaternion representation.
     * The conversion uses the ZYX rotation order (roll, then yaw, then pitch).
     * 
     * @param EulerAngles Vector3 containing rotation angles in radians (X=pitch, Y=yaw, Z=roll)
     * @return Quaternion representing the same rotation
     */
    inline F_Quaternion_F32 EulerAnglesToQuaternion(PA_Vector3_F32 EulerAngles) noexcept
    {
        const F32 HalfPitch = EulerAngles.X * F32(0.5);
        const F32 HalfYaw   = EulerAngles.Y * F32(0.5);
        const F32 HalfRoll  = EulerAngles.Z * F32(0.5);

        const F32 CP = Cos(HalfPitch);
        const F32 SP = Sin(HalfPitch);

        const F32 CY = Cos(HalfYaw);
        const F32 SY = Sin(HalfYaw);

        const F32 CR = Cos(HalfRoll);
        const F32 SR = Sin(HalfRoll);

        return {
            CR * SP * CY + SR * CP * SY,
            CR * CP * SY - SR * SP * CY,
            SR * CP * CY - CR * SP * SY,
            CR * CP * CY + SR * SP * SY
        };
    }
}