#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"
#include "Abytek/Functions/ACos.hpp"
#include "Abytek/Functions/Normalize.hpp"
#include "Abytek/Functions/Cross.hpp"
#include "Abytek/Operators/QuaternionScalar.hpp"


namespace Abytek
{
    /**
     * @brief Converts a normalized quaternion to an axis-angle representation
     * 
     * Extracts the rotation axis and angle from a quaternion. The quaternion must be normalized
     * for correct results. The returned Vector4 contains the normalized axis in the first three
     * components (X, Y, Z) and the rotation angle in radians in the W component.
     *
     * @param NormalizedQ A normalized quaternion representing a rotation
     * @return A Vector4 where (X,Y,Z) is the normalized rotation axis and W is the rotation angle in radians
     */
    inline F_Vector4_F32 AxisAndAngle(PA_Quaternion_F32 NormalizedQ) noexcept 
    {
        const F32 W = std::clamp(
            NormalizedQ.W,
            F32(-1),
            F32(1)
        );

        const F32 Angle = F32(2) * std::acos(W);

        const F32 SinHalfAngle = std::sqrt(
            std::max(
                F32(0),
                F32(1) - W * W
            )
        );

        if (SinHalfAngle < F32(1e-6))
        {
            // Axis is undefined for the identity rotation.
            // Pick a stable default axis.
            return {
                F32(0),
                F32(1),
                F32(0),
                F32(0)
            };
        }

        const F32 InvSinHalfAngle = F32(1) / SinHalfAngle;

        return {
            NormalizedQ.X * InvSinHalfAngle,
            NormalizedQ.Y * InvSinHalfAngle,
            NormalizedQ.Z * InvSinHalfAngle,
            Angle
        };
    }
}