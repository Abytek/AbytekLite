#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    namespace AndroidJavaItems
    {
        B8 IsReady();
    }
    namespace AndroidJavaItems::BridgeContext
    {
        jclass GetClass();
    }
}
#endif