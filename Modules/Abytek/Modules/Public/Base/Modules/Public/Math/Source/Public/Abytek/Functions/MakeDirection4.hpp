/**
 * @file MakeDirection4.hpp
 * @brief Function to convert a 3D direction vector to a 4D homogeneous direction vector
 * 
 * Provides a utility function to convert a 3D direction vector to a 4D homogeneous
 * direction vector by setting the W component to 0.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek
{
    /**
     * @brief Creates a 4D homogeneous direction vector from a 3D direction vector
     * 
     * Converts a 3D direction vector to a 4D homogeneous representation by setting the
     * W component to 0. Direction vectors in homogeneous coordinates have a W component
     * of 0 to ensure they're unaffected by translations when transformed.
     * 
     * @param Direction3 The 3D direction vector to convert
     * @return A 4D homogeneous direction vector with W=0
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 MakeDirection4(PA_Vector3_F32 Direction3) noexcept
    {
        return {
            Direction3,
            0.0f
        };
    }
}