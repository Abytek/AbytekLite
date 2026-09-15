#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"
#include "Abytek/Functions/Normalize.hpp"
#include "Abytek/Functions/Cross.hpp"
#include "Abytek/Functions/Dot.hpp"
#include "Abytek/Functions/QuaternionIdentity.hpp"
#include "Abytek/Functions/QuaternionInverse.hpp"
#include "Abytek/Functions/Lerp.hpp"
#include "Abytek/Functions/AxisAndAngle.hpp"
#include "Abytek/Functions/AxisAngleToQuaternion.hpp"
#include "Abytek/Operators/VectorVector.hpp"
#include "Abytek/Operators/QuaternionScalar.hpp"
#include "Abytek/Operators/QuaternionQuaternion.hpp"


namespace Abytek
{
    /**
     * @brief Performs spherical linear interpolation between two normalized quaternions
     * 
     * Performs a smooth rotation interpolation between two unit quaternions along
     * the shortest arc on the 4D unit sphere. This ensures constant angular velocity
     * during interpolation, unlike linear interpolation.
     * 
     * The implementation finds the angular difference between quaternions using
     * multiplication with the inverse, extracts axis-angle representation, 
     * interpolates the angle, and applies the partial rotation.
     * 
     * @param a First quaternion (normalized)
     * @param b Second quaternion (normalized)
     * @param t Interpolation parameter in range [0,1]
     * @return F_Quaternion_F32 Interpolated quaternion (normalized)
     */
    inline F_Quaternion_F32 SLerpNormalized(PA_Quaternion_F32 a, PA_Quaternion_F32 b, F32 t) noexcept {

        F_Quaternion_F32 delta_q = b * Inverse(a);

        F_Vector4_F32 aaa = AxisAndAngle(delta_q);
        aaa.W = Lerp(0.0f, aaa.W, t);

        F_Quaternion_F32 add_q = AxisAngleToQuaternion(aaa);

        return add_q * a;
    }
    
    /**
     * @brief Performs spherical linear interpolation between two quaternions
     * 
     * Normalizes the input quaternions, performs SLerp, then scales
     * the result according to the linear interpolation of quaternion lengths.
     * This preserves both rotation and scale interpolation.
     * 
     * @param a First quaternion
     * @param b Second quaternion
     * @param t Interpolation parameter in range [0,1]
     * @return F_Quaternion_F32 Interpolated quaternion
     */
    inline F_Quaternion_F32 SLerp(PA_Quaternion_F32 a, PA_Quaternion_F32 b, F32 t) noexcept {

        F32 a_length = Length(a);
        F32 b_length = Length(b);

        F_Quaternion_F32 na = a / a_length;
        F_Quaternion_F32 nb = b / b_length;

        return SLerpNormalized(na, nb, t) * Lerp(a_length, b_length, t);
    }
}