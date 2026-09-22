#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHIProcessFlushFlag : U8
    {
        NONE = 0x0,
        COMPILE = 0x1,
        EXECUTE = 0x2 | COMPILE,
        
        DEFAULT = EXECUTE
    };
}