#pragma once

#include "Abytek/ReflectionCommon.hpp"


namespace Abytek
{
    template<typename __F>
    struct TF_ReflectionTypeHandle;
    
    namespace Internal::ReflectionTypeHandle
    {
        template<typename __F>
        struct TH_Check
        {
            static constexpr B8 Value = false;
        };
        template<typename __F>
        struct TH_Check<TF_ReflectionTypeHandle<__F>>
        {
            static constexpr B8 Value = true;
        };
    }
    template<typename __F>
    static constexpr B8 IsReflectionTypeHandle()
    {
        return Internal::ReflectionTypeHandle::TH_Check<__F>::Value;
    }
}