#include "Abytek/Android/Platform.hpp"
#include "Abytek/Android/JNIManager.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    F_AndroidPlatform::F_AndroidPlatform()
    {
        _JNIManager = TU<F_AndroidJNIManager>()();
    }
    F_AndroidPlatform::~F_AndroidPlatform()
    {
        _JNIManager = {};
    }

    void F_AndroidPlatform::OnBeginCurrentThread()
    {
        _JNIManager->AttachToCurrentThread();
    }
    void F_AndroidPlatform::OnEndCurrentThread()
    {
        _JNIManager->DeattachFromCurrentThread();
    }
}
#endif