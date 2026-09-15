/**
 * @file LinearToAffine.hpp
 * @brief Conversion from linear (3x3) to affine (4x4) transformation matrix
 * 
 * Provides a function to convert a 3x3 linear transformation matrix to a 4x4 affine
 * transformation matrix with an identity translation component.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek
{
    /**
     * @brief Converts a 3x3 linear transformation matrix to a 4x4 affine transformation matrix
     * 
     * Creates a 4x4 affine transformation matrix from a 3x3 linear transformation matrix.
     * The linear part is preserved in the upper-left 3x3 submatrix, the translation part
     * is set to zero (identity), and the bottom row remains to be set by the caller.
     * 
     * @param Affine The 3x3 linear transformation matrix
     * @return A 4x4 affine transformation matrix with zero translation
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 LinearToAffine(PA_Matrix3x3_F32 Affine) noexcept
    {
        return {
            { Affine.A, 0.0f },
            { Affine.B, 0.0f },
            { Affine.C, 0.0f },
            F_Vector4_F32::Future()
        };
    }
}