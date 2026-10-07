#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/CrtUseAllocatorTypedefs.hpp"


namespace Abytek
{
    struct ABYTEK_BASE_CORE_API H_UUID
    {
        static F_Text Generate();
    };
}