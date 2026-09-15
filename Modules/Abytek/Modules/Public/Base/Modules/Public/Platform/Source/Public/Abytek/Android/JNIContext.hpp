#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    struct ABYTEK_BASE_PLATFORM_API F_AndroidJNIContext
    {
        JNIEnv* Env = nullptr;
        B8 Owned = true;

        static F_AndroidJNIContext& GetCurrent();
        static void AttachToCurrentThread(B8 Get = false);
        static void DeattachFromCurrentThread();
    };
}
#endif