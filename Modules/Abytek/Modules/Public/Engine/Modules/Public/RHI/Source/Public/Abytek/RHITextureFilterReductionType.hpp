#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHITextureFilterReductionType : U8
    {
        NONE,
        STANDARD,
        COMPARE,
        MIN,
        MAX,
        DEFAULT = STANDARD,
    };
}