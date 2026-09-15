#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    class A_RHITemplate;
    
    using F_RHITemplateHashCode = Sz;

    enum class E_RHITemplateHashMode : U8
    {
        NONE,
        CONFIG_BASED,
        USER_DEFINED,
        OBJECT_BASED
    };
    constexpr F_RHITemplateHashCode INVALID_RHI_TEMPLATE_HASH_CODE = HashCombineU64(0, static_cast<U64>(E_RHITemplateHashMode::NONE));
    constexpr F_RHITemplateHashCode RHITemplateFinalizeHashCode(E_RHITemplateHashMode HashMode, F_RHITemplateHashCode RawHashCode)
    {
        F_RHITemplateHashCode Result = 0;
        Result = HashCombineU64(Result, static_cast<U64>(HashMode));
        Result = HashCombineU64(Result, RawHashCode);
        return Result;
    }
    constexpr F_RHITemplateHashCode RHITemplateUserDefinedHashCode(F_RHITemplateHashCode UserDefinedHashCode)
    {
        return RHITemplateFinalizeHashCode(
            E_RHITemplateHashMode::USER_DEFINED,
            UserDefinedHashCode
        );
    }
    template<typename __F_Config>
    F_RHITemplateHashCode RHITemplateConfigBasedHashCodeAdvanced(
        F_RHITemplateHashCode ConfigHashCode,
        const TF_Optional<F_RHITemplateHashCode>& BaseHashCode = {}
    )
    {
        F_RHITemplateHashCode RawHashCode = 0;
        if (BaseHashCode)
        {
            RawHashCode = HashCombineU64(
                RawHashCode,
                1U
            );
            RawHashCode = HashCombineU64(
                RawHashCode,
                *BaseHashCode
            );
        }
        else
        {
            RawHashCode = HashCombineU64(
                RawHashCode,
                0U
            );
        }
        RawHashCode = HashCombineU64(
            RawHashCode,
            TypeHashCode<__F_Config>
        );
        RawHashCode = HashCombineU64(
            RawHashCode,
            ConfigHashCode
        );
        return RHITemplateFinalizeHashCode(
            E_RHITemplateHashMode::CONFIG_BASED,
            RawHashCode
        );
    }
    template<typename __F_Config>
    F_RHITemplateHashCode RHITemplateConfigBasedHashCode(
        const __F_Config& Config,
        const TF_Optional<F_RHITemplateHashCode>& BaseHashCode = {}
    )
    {
        return RHITemplateConfigBasedHashCodeAdvanced<__F_Config>(
            boost::hash<__F_Config>()(Config),
            BaseHashCode    
        );
    }
    inline F_RHITemplateHashCode RHITemplateObjectBasedHashCode(const A_RHITemplate* TemplateRawPtr)
    {
        return RHITemplateFinalizeHashCode(
            E_RHITemplateHashMode::OBJECT_BASED,
            (F_RHITemplateHashCode)TemplateRawPtr
        );
    }
    
    enum class E_RHITemplateHashCodeLockStatus : U8
    {
        NONE,
        FAILED,
        NEED_TO_CREATE,
        CREATED,
    };
}