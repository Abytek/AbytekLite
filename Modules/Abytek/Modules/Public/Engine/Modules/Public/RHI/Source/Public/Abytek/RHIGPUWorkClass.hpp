#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHIGPUWorkClass
    {
        NONE,
  
        DIRECT,
        COMPUTE,
        BLIT,
  
        DEFAULT = DIRECT
    };
}