#include "Abytek/Android/JNIContext.hpp"
#include "Abytek/Android/JNIManager.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    namespace Internal::AndroidJNIContext
    {
        thread_local F_AndroidJNIContext Current;
    }
    F_AndroidJNIContext& F_AndroidJNIContext::GetCurrent()
    {
        return Internal::AndroidJNIContext::Current;
    }
    void F_AndroidJNIContext::AttachToCurrentThread(B8 Get)
    {
        auto& Current = GetCurrent();
        auto VM = F_AndroidJNIManager::GetInstance()->GetVM();
        jint Status = VM->GetEnv((void**)&Current.Env, JNI_VERSION_1_6);
        if (Status == JNI_OK)
        {
            Current.Owned = false;
        }
        else
        {
            Status = F_AndroidJNIManager::GetInstance()->GetVM()->AttachCurrentThread(&Current.Env, nullptr);
            ABYTEK_BASE_PLATFORM_ASSERT(Status == JNI_OK) << "Cannot attach JNI to current thread";
            Current.Owned = true;
        }
    }
    void F_AndroidJNIContext::DeattachFromCurrentThread()
    {
        auto& Current = GetCurrent();
        if (!Current.Owned)
        {
            return;
        }
        auto VM = F_AndroidJNIManager::GetInstance()->GetVM();
        auto Status = VM->DetachCurrentThread();
        ABYTEK_BASE_PLATFORM_ASSERT(Status == JNI_OK) << "Cannot detach JNI from current thread";
        Current.Env = nullptr;
    }
}
#endif