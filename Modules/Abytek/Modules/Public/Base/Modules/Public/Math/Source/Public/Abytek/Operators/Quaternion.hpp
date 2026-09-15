/**
 * @file Quaternion.hpp
 * @brief Defines basic quaternion operations.
 * 
 * This file contains the unary negation operator for quaternions of F32 (floating point) type.
 * Other quaternion operations are defined in specialized files like QuaternionMatrix, 
 * QuaternionQuaternion, QuaternionScalar, and QuaternionVector.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek::Internal::MathOperators
{
    /**
     * @brief Unary negation operator for F32 quaternions.
     * @param A The quaternion to negate.
     * @return The negated quaternion.
     */
    ABYTEK_FORCE_INLINE Abytek::F_Quaternion_F32 operator - (Abytek::PA_Quaternion_F32 A) noexcept
    {
        return Abytek::H_Data4::Minus<Abytek::F32, Abytek::F_QuaternionConfig>(A);
    }
}