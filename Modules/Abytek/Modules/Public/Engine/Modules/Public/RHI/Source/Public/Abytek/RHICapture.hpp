#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHICaptureFlag : U8
    {
        NONE = 0x0,
        GPU = 0x1,
        TIMING = 0x2,
        
        DEFAULT = GPU
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RHICaptureFlag);
}