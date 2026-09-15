#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHICompareOperation : U16
    {
        NONE,
        NEVER,
        LESS,
        EQUAL,
        LESS_EQUAL,
        GREATER,
        NOT_EQUAL,
        GREATER_EQUAL,
        ALWAYS
    };
}