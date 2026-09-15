#pragma once

#include "Abytek/ReflectionType.hpp"
#include "Abytek/ReflectionSession.hpp"


namespace Abytek
{       
    template<typename __F_Type>
    TW_Valid<F_ReflectionType> F_ReflectionType::ReflectReferenced()
    {
        auto Result = GetContext()->GetCurrentSession()->_Reflect<__F_Type>();
        GBMEnsureReference(Result);
        return Result;
    }
}