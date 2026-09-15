#include "Abytek/Android/JavaItems.hpp"
#include "Abytek/Android/JNIContext.hpp"
#include "Abytek/Android/JNIManager.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    namespace Internal::AndroidJavaItems
    {
        B8 IsReady = false;
    }
    namespace AndroidJavaItems
    {
        B8 IsReady()
        {
            return Internal::AndroidJavaItems::IsReady;
        }
    }
    namespace Internal::AndroidJavaItems::BridgeContext
    {
        jclass Class = nullptr;
    }
    namespace AndroidJavaItems::BridgeContext
    {
        jclass GetClass()
        {
            return Internal::AndroidJavaItems::BridgeContext::Class;
        }
    }

    extern "C" JNIEXPORT void JNICALL Java_com_abytek_BridgeContext_onInitialized(
        JNIEnv* env, jclass clazz
    )
    {
        Internal::AndroidJavaItems::BridgeContext::Class = (jclass)env->NewGlobalRef(clazz);
        Internal::AndroidJavaItems::IsReady = true;
    }
    extern "C" JNIEXPORT void JNICALL Java_com_abytek_BridgeContext_onDeinitialized(
            JNIEnv* env, jclass clazz
    )
    {
        Internal::AndroidJavaItems::IsReady = false;
        env->DeleteGlobalRef(
            Internal::AndroidJavaItems::BridgeContext::Class
        );
        Internal::AndroidJavaItems::BridgeContext::Class = nullptr;
    }
}
#endif