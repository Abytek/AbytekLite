#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHISubmissionListOrder
    {
        SEQUENTIAL,
        CONCURRENT,

        DEFAULT = SEQUENTIAL
    };
}