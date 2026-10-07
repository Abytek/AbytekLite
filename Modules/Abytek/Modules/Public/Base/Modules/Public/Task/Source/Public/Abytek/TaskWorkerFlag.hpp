#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"


namespace Abytek
{
    enum class E_TaskWorkerFlag : U8
    {
        NONE = 0x0,
        LOW_FREQUENCY = 0x1,
        MEDIUM_FREQUENCY = 0x2,
        HIGH_FREQUENCY = 0x4,
        MAIN_THREAD = 0x8,
        DEDICATED_THREAD = 0x10,
        
        DEFAULT = NONE
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_TaskWorkerFlag);
}