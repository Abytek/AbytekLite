#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    struct F_AndroidExecutableData
    {
        android_app* AndroidApp = nullptr;
    };
}
#endif