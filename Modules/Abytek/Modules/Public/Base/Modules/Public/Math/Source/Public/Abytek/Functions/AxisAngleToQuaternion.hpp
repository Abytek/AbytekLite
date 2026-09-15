#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"
#include "Abytek/Functions/Cos.hpp"
#include "Abytek/Functions/Sin.hpp"


namespace Abytek
{
    /**
     * @brief Converts an axis and angle representation to a quaternion
     * 
     * Creates a quaternion representing a rotation around the specified axis by the given angle.
     * The axis should be normalized for correct results.
     *
     * @param Axis A normalized 3D vector representing the rotation axis
     * @param Angle The rotation angle in radians
     * @return A quaternion representing the rotation
     */
    inline F_Quaternion_F32 AxisAngleToQuaternion(PA_Vector3_F32 Axis, F32 Angle) noexcept
    {
        const F32 HalfAngle = Angle * F32(0.5);

        const F32 SinHalfAngle = Sin(HalfAngle);
        const F32 CosHalfAngle = Cos(HalfAngle);

        return F_Vector4_F32 {
            Axis * SinHalfAngle,
            CosHalfAngle
        }.RebindConfig<F_QuaternionConfig>();
    }
    
    /**
     * @brief Converts an axis-angle representation to a quaternion
     * 
     * Creates a quaternion representing a rotation where the first three components of AxisAndAngle
     * (X, Y, Z) represent the normalized rotation axis, and the fourth component (W) represents the
     * rotation angle in radians.
     *
     * @param AxisAndAngle A 4D vector where (X,Y,Z) is the normalized rotation axis and W is the rotation angle in radians
     * @return A quaternion representing the rotation
     */
    inline F_Quaternion_F32 AxisAngleToQuaternion(PA_Vector4_F32 AxisAndAngle) noexcept
    {
        const F32 HalfAngle = AxisAndAngle.W * F32(0.5);

        const F32 SinHalfAngle = Sin(HalfAngle);
        const F32 CosHalfAngle = Cos(HalfAngle);

        return F_Vector4_F32 {
            AxisAndAngle.StaticPermute<0, 1, 2>() * SinHalfAngle,
            CosHalfAngle
        }.RebindConfig<F_QuaternionConfig>();
    }
}