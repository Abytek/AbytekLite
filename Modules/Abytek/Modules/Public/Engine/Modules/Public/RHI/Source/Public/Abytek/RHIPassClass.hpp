#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHIPassClass : U8
    {
        NONE,
        
        GRAPHICS,
        COMPUTE,
        BLIT
    };
}