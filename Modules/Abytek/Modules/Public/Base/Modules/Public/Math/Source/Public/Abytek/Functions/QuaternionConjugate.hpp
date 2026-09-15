#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * Computes the conjugate of a quaternion.
     * The conjugate of a quaternion (x, y, z, w) is (-x, -y, -z, w).
     * For unit quaternions, the conjugate is also the inverse.
     * 
     * @param Quat Input quaternion
     * @return Conjugate of the input quaternion
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Conjugate(PA_Quaternion_F32 Quat) noexcept
    {
        return {
            -Quat.X,
            -Quat.Y,
            -Quat.Z,
            Quat.W
        };
    }
}