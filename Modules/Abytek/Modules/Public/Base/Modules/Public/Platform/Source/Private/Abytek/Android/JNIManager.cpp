#include "Abytek/Android/JNIManager.hpp"
#include "Abytek/Android/JNIContext.hpp"
#include "Abytek/Android/JavaItems.hpp"
#include "Abytek/Executable.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_AndroidJNIManager);

    F_AndroidJNIManager::F_AndroidJNIManager()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();

        auto AndroidApp = F_Executable::GetInstance().AndroidData.AndroidApp;
        _VM = AndroidApp->activity->vm;
        while (!AndroidJavaItems::IsReady());

        AttachToCurrentThread();
    }
    F_AndroidJNIManager::~F_AndroidJNIManager()
    {
        DeattachFromCurrentThread();
    }

    void F_AndroidJNIManager::AttachToCurrentThread()
    {
        F_AndroidJNIContext::AttachToCurrentThread();
    }
    void F_AndroidJNIManager::DeattachFromCurrentThread()
    {
        F_AndroidJNIContext::DeattachFromCurrentThread();
    }
}
#endif