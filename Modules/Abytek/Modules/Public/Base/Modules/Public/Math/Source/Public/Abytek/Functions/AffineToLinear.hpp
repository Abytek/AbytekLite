#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"

/**
 * @file AffineToLinear.hpp
 * @brief Provides functionality to extract the linear part from an affine transformation matrix.
 * 
 * This file contains a function to convert a 4x4 affine transformation matrix to
 * its 3x3 linear transformation component by extracting the top-left 3x3 submatrix,
 * discarding the translation component and homogeneous coordinate.
 */
namespace Abytek
{
    /**
     * @brief Extracts the linear transformation component from an affine transformation matrix.
     * @param Affine The 4x4 affine transformation matrix.
     * @return The 3x3 linear transformation matrix (top-left submatrix of the affine matrix).
     * 
     * In an affine transformation matrix, the top-left 3x3 submatrix represents
     * the linear transformation part (rotation, scaling, and/or shearing),
     * while the fourth column represents translation.
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 AffineToLinear(PA_Matrix4x4_F32 Affine) noexcept
    {
        return Affine.GetSmallerTL();
    }
}