#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Functions/Data2_NonSIMD.hpp"
#include "Abytek/Functions/Data3_NonSIMD.hpp"
#include "Abytek/Functions/Data4_NonSIMD.hpp"


namespace Abytek
{
    /**
     * @brief Computes the 2D cross product of two vectors
     * 
     * For 2D vectors, the cross product is a scalar value equal to the area of the parallelogram
     * formed by the two vectors. It is calculated as A.X * B.Y - A.Y * B.X.
     *
     * @param A First vector
     * @param B Second vector
     * @return The scalar cross product of A and B
     */
    ABYTEK_FORCE_INLINE F32 Cross(PA_Vector2_F32 A, PA_Vector2_F32 B) noexcept
    {
        return A.X * B.Y - A.Y * B.X;
    }
    
    /**
     * @brief Computes the 3D cross product of two vectors
     * 
     * Returns a vector perpendicular to both input vectors, with magnitude equal to the area of the
     * parallelogram formed by the two vectors.
     *
     * @param A First vector
     * @param B Second vector
     * @return The vector cross product of A and B
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Cross(PA_Vector3_F32 A, PA_Vector3_F32 B) noexcept
    {
        return {
            A.Y * B.Z - A.Z * B.Y,
            A.Z * B.X - A.X * B.Z,
            A.X * B.Y - A.Y * B.X
        };
    }
    
    /**
     * @brief Computes the 4D cross product of three vectors
     * 
     * In 4D space, the cross product requires three vectors as input and produces a fourth vector
     * that is orthogonal to the three input vectors.
     *
     * @param A First vector
     * @param B Second vector
     * @param C Third vector
     * @return The 4D cross product, a vector orthogonal to A, B, and C
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Cross(PA_Vector4_F32 A, PA_Vector4_F32 B, PA_Vector4_F32 C) noexcept
    {
        return {
            (B.Z * C.W - B.W * C.Z) * A.Y - (B.Y * C.W - B.W * C.Y) * A.Z + (B.Y * C.Z - B.Z * C.Y) * A.W,
            (B.W * C.Z - B.Z * C.W) * A.X - (B.W * C.X - B.X * C.W) * A.Z + (B.Z * C.X - B.X * C.Z) * A.W,
            (B.Y * C.W - B.W * C.Y) * A.X - (B.X * C.W - B.W * C.X) * A.Y + (B.X * C.Y - B.Y * C.X) * A.W,
            (B.Z * C.Y - B.Y * C.Z) * A.X - (B.Z * C.X - B.X * C.Z) * A.Y + (B.Y * C.X - B.X * C.Y) * A.Z
        };
    }
}