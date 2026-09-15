/**
 * @file MakePosition4.hpp
 * @brief Function to convert a 3D position vector to a 4D homogeneous position vector
 * 
 * Provides a utility function to convert a 3D position vector to a 4D homogeneous
 * position vector by setting the W component to 1.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek
{
    /**
     * @brief Creates a 4D homogeneous position vector from a 3D position vector
     * 
     * Converts a 3D position vector to a 4D homogeneous representation by setting the
     * W component to 1. Position vectors in homogeneous coordinates have a W component
     * of 1 to ensure they're properly affected by translations when transformed.
     * 
     * @param Position3 The 3D position vector to convert
     * @return A 4D homogeneous position vector with W=1
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 MakePosition4(PA_Vector3_F32 Position3) noexcept
    {
        return {
            Position3,
            1.0f
        };
    }
}