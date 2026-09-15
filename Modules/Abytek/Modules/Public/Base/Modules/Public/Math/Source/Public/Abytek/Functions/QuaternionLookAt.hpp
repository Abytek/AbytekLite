#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"
#include "Abytek/Functions/Normalize.hpp"
#include "Abytek/Functions/Cross.hpp"
#include "Abytek/Functions/Dot.hpp"
#include "Abytek/Functions/QuaternionIdentity.hpp"
#include "Abytek/Operators/VectorVector.hpp"
#include "Abytek/Operators/QuaternionScalar.hpp"


namespace Abytek
{
    /**
     * @brief Creates a quaternion that represents the rotation from one direction to another
     * 
     * Constructs a quaternion that will rotate the FromDirection vector to align with
     * the ToDirection vector. If the directions are nearly parallel (within Tolerance),
     * returns an identity quaternion.
     * 
     * The function calculates the rotation axis using cross product and computes
     * the angle of rotation based on the dot product of the normalized vectors.
     * 
     * @param FromDirection Source direction vector
     * @param ToDirection Target direction vector
     * @param Tolerance Angular tolerance value, default is DEFAULT_TOLERANCE<F32>
     * @return F_Quaternion_F32 Quaternion representing the rotation between directions
     */
    inline F_Quaternion_F32 QuaternionLookAt(PA_Vector3_F32 FromDirection, PA_Vector3_F32 ToDirection, F32 Tolerance = DEFAULT_TOLERANCE<F32>) noexcept 
    {
        // equal to axis * sin(angle)
        F_Vector3_F32 AxisSinAngle = Cross(FromDirection, ToDirection);

        // if FromDirection is equal to ToDirection, returns identity quaternion
        if(LengthSquare(AxisSinAngle) <= Tolerance)
            return Identity<F_Quaternion_F32>();

        // cos(angle) = [cos(angle/2)]^2 - [sin(angle/2)]^2
        //            = 2[cos(angle/2)]^2 - 1
        //
        // <=> cos(angle/2) = sqrt(1/2 + cos(angle)/2)
        F32 DotHalf = Dot(FromDirection, ToDirection) * 0.5f;
        F32 CosHalfAngle = SquareRoot(0.5f + DotHalf);

        // this variable will be multiplied with AxisSinAngle to get axis * sin(angle/2)
        //
        // AxisSinAngle * AxisSinAngle_multiplier = axis * sin(angle/2)
        //
        // <=> axis * sin(angle) * AxisSinAngle_multiplier = axis * sin(angle/2)
        //
        // <=> AxisSinAngle_multiplier = axis * sin(angle/2) / sin(angle)
        //
        // <=> AxisSinAngle_multiplier = sin(angle/2) / [2sin(angle/2)cos(angle/2)]
        // <=> AxisSinAngle_multiplier = (1 / 2) / cos(angle/2)
        F32 AxisSinAngle_multiplier = 0.5f / CosHalfAngle;

        // xyz = axis * sin(angle/2)
        // w = cos(angle/2)
        return F_Vector4_F32 {
            AxisSinAngle * AxisSinAngle_multiplier,
            CosHalfAngle
        }.RebindConfig<F_QuaternionConfig>();
    }
}