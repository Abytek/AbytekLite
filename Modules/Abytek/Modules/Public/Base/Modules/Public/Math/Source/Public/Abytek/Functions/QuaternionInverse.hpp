#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Quaternion.hpp"
#include "Abytek/Functions/QuaternionConjugate.hpp"
#include "Abytek/Operators/QuaternionScalar.hpp"


namespace Abytek
{
    /**
     * @brief Computes the inverse of a quaternion
     * 
     * The inverse of a quaternion Q is such that Q * Q^(-1) = identity quaternion.
     * For unit quaternions, the inverse equals the conjugate. For non-unit quaternions,
     * we divide the conjugate by the squared norm.
     * 
     * @param Quat Input quaternion to invert
     * @return F_Quaternion_F32 The inverse quaternion
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Inverse(PA_Quaternion_F32 Quat) noexcept
    {
        return Conjugate(Quat) / Length(Quat);
    }
}