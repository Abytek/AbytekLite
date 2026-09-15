#pragma once

#include "Abytek/Engine.Window.prerequisites.hpp"


namespace Abytek
{
    enum class E_WindowAPI
    {
        NONE,
        WINDOWS,
        ANDROID_
    };

    ABYTEK_FORCE_INLINE F_Name WindowGetAPIName(E_WindowAPI API) noexcept
    {
        switch (API)
        {
        case E_WindowAPI::NONE:
            return {};
        case E_WindowAPI::WINDOWS:
            return ABYTEK_NAME("Windows");
        case E_WindowAPI::ANDROID_:
            return ABYTEK_NAME("Android");
        }
        return {};
    }
    
    ABYTEK_FORCE_INLINE TF_Vector<E_WindowAPI> WindowGetDefaultAPIs()
    {
        TF_Vector<E_WindowAPI> Result;
#if defined(ABYTEK_PLATFORM_WINDOWS)
        Result.push_back(E_WindowAPI::WINDOWS);
#endif
#if defined(ABYTEK_PLATFORM_ANDROID)
        Result.push_back(E_WindowAPI::ANDROID_);
#endif
        return boost::move(Result);
    }
}