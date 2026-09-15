#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/IdentityTemplate.hpp"


namespace Abytek
{
    /**
     * Template specialization for creating an identity quaternion.
     * The identity quaternion is (0, 0, 0, 1) which represents no rotation.
     * 
     * @return An identity quaternion
     */
    template<>
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Identity<F_Quaternion_F32>() noexcept
    {
        return F_Quaternion_F32::Future();
    }
}