/**
 * @file Quaternion.hpp
 * @brief Quaternion type definitions for the Abytek math system.
 * 
 * This file defines quaternion types using the Data template,
 * with both fixed-layout (TF_*) and platform-aligned (TPA_*) variants.
 * Quaternions are 4-component mathematical objects commonly used for 
 * representing 3D rotations.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"


namespace Abytek
{
    /**
     * @struct F_QuaternionConfig
     * @brief Configuration struct for quaternion types.
     */
    struct F_QuaternionConfig : TA_DataConfig<TA_DefaultData>
    {
    };
    
    /**
     * @typedef TF_Quaternion
     * @brief Quaternion with fixed layout.
     * @tparam __F_Element The scalar type used for quaternion components.
     */
    template<typename __F_Element>
    using TF_Quaternion = TF_Data<4, 1, __F_Element, F_QuaternionConfig>;
    using F_Quaternion = TF_Quaternion<ABYTEK_DEFAULT_FP_TYPE>;
    using F_Quaternion_F32 = TF_Quaternion<F32>;
    
    /**
     * @typedef TPA_Quaternion
     * @brief Quaternion with platform-aligned layout.
     * @tparam __F_Element The scalar type used for quaternion components.
     */
    template<typename __F_Element>
    using TPA_Quaternion = TPA_Data<4, 1, __F_Element, F_QuaternionConfig>;
    using PA_Quaternion = TPA_Quaternion<ABYTEK_DEFAULT_FP_TYPE>;
    using PA_Quaternion_F32 = TPA_Quaternion<F32>;
}