#pragma once

#include "Abytek/Platform.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class F_AndroidJNIManager;

    class ABYTEK_BASE_PLATFORM_API F_AndroidPlatform : public A_Platform
    {
    private:
        TU<F_AndroidJNIManager> _JNIManager;

    public:

    public:
        F_AndroidPlatform();
        virtual ~F_AndroidPlatform() override;

    public:
        virtual void OnBeginCurrentThread() override;
        virtual void OnEndCurrentThread() override;
    };
}
#endif