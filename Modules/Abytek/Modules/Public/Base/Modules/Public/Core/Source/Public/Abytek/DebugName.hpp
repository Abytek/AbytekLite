#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/Name.hpp"
#include "Abytek/CrtUseAllocatorTypedefs.hpp"


namespace Abytek
{
    using F_DebugName = F_Name;
}


#define ABYTEK_DEBUG_NAME(...) ABYTEK_NAME(__VA_ARGS__)