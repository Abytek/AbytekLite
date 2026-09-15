#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_IHIAPI
    {
        NONE,
        WINDOWS,
        ANDROID_
    };

    ABYTEK_FORCE_INLINE F_Name IHIGetAPIName(E_IHIAPI API) noexcept
    {
        switch (API)
        {
        case E_IHIAPI::NONE:
            return {};
        case E_IHIAPI::WINDOWS:
            return ABYTEK_NAME("Windows");
        case E_IHIAPI::ANDROID_:
            return ABYTEK_NAME("Android");
        }
        return {};
    }
    
    ABYTEK_FORCE_INLINE TF_Vector<E_IHIAPI> IHIGetDefaultAPIs()
    {
        TF_Vector<E_IHIAPI> Result;
#if defined(ABYTEK_PLATFORM_WINDOWS)
        Result.push_back(E_IHIAPI::WINDOWS);
#endif
#if defined(ABYTEK_PLATFORM_ANDROID)
        Result.push_back(E_IHIAPI::ANDROID_);
#endif
        return boost::move(Result);
    }
}