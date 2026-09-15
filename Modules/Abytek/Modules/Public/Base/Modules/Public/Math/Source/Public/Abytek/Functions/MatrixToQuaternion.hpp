#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Operators/QuaternionMatrix.hpp"
#include "Abytek/Functions/SquareRoot.hpp"


namespace Abytek
{
    /**
     * Converts a 3x3 rotation matrix to a quaternion.
     * Uses a numerically stable approach to avoid precision issues.
     * 
     * @param m The input 3x3 rotation matrix
     * @return Quaternion representing the same rotation
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 LinearMatrixToQuaternion(PA_Matrix3x3_F32 m) noexcept
    {
        F_Quaternion_F32 q;
        float r22 = m.C.Z;
        if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
        {
            float dif10 = m.B.Y - m.A.X;
            float omr22 = 1.f - r22;
            if (dif10 <= 0.f)  // x^2 >= y^2
            {
                float fourXSqr = omr22 - dif10;
                float inv4x = 0.5f / SquareRoot(fourXSqr);
                q.X = fourXSqr * inv4x;
                q.Y = (m.A.Y + m.B.X) * inv4x;
                q.Z = (m.A.Z + m.C.X) * inv4x;
                q.W = (m.B.Z - m.C.Y) * inv4x;
            }
            else  // y^2 >= x^2
            {
                float fourYSqr = omr22 + dif10;
                float inv4y = 0.5f / SquareRoot(fourYSqr);
                q.X = (m.A.Y + m.B.X) * inv4y;
                q.Y = fourYSqr * inv4y;
                q.Z = (m.B.Z + m.C.Y) * inv4y;
                q.W = (m.C.X - m.A.Z) * inv4y;
            }
        }
        else  // z^2 + w^2 >= x^2 + y^2
        {
            float sum10 = m.B.Y + m.A.X;
            float opr22 = 1.f + r22;
            if (sum10 <= 0.f)  // z^2 >= w^2
            {
                float fourZSqr = opr22 - sum10;
                float inv4z = 0.5f / SquareRoot(fourZSqr);
                q.X = (m.A.Z + m.C.X) * inv4z;
                q.Y = (m.B.Z + m.C.Y) * inv4z;
                q.Z = fourZSqr * inv4z;
                q.W = (m.A.Y - m.B.X) * inv4z;
            }
            else  // w^2 >= z^2
            {
                float fourWSqr = opr22 + sum10;
                float inv4w = 0.5f / SquareRoot(fourWSqr);
                q.X = (m.B.Z - m.C.Y) * inv4w;
                q.Y = (m.C.X - m.A.Z) * inv4w;
                q.Z = (m.A.Y - m.B.X) * inv4w;
                q.W = fourWSqr * inv4w;
            }
        }
        return q;
    }
    
    /**
     * Converts a 4x4 affine transformation matrix to a quaternion.
     * Extracts the 3x3 rotation component before conversion.
     * 
     * @param M The input 4x4 affine matrix
     * @return Quaternion representing the rotation component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 AffineMatrixToQuaternion(PA_Matrix4x4_F32 M) noexcept
    {
        return LinearMatrixToQuaternion(AffineToLinear(M));
    }
}