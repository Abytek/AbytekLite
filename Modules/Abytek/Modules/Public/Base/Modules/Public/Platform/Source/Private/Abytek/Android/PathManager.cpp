#include "Abytek/Android/PathManager.hpp"
#include "Abytek/Android/JNIContext.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    namespace Internal::AndroidJavaItems::PathManager_Bridge
    {
        jclass Class = nullptr;
    }
    namespace AndroidJavaItems::PathManager_Bridge
    {
        jclass GetClass()
        {
            return Internal::AndroidJavaItems::PathManager_Bridge::Class;
        }
    }

    extern "C" JNIEXPORT void JNICALL Java_com_abytek_base_platform_PathManager_1Bridge_onInitialized(
        JNIEnv* env, jclass clazz
    )
    {
        Internal::AndroidJavaItems::PathManager_Bridge::Class = (jclass)env->NewGlobalRef(clazz);
    }
    extern "C" JNIEXPORT void JNICALL Java_com_abytek_base_platform_PathManager_1Bridge_onDeinitialized(
        JNIEnv* env, jclass clazz
    )
    {
        env->DeleteGlobalRef(
            Internal::AndroidJavaItems::PathManager_Bridge::Class
        );
        Internal::AndroidJavaItems::PathManager_Bridge::Class = nullptr;
    }

    F_AndroidPathManager::F_AndroidPathManager()
    {
        _QueryDirectories();
    }
    F_AndroidPathManager::~F_AndroidPathManager()
    {
    }

    TF_Optional<F_Text> F_AndroidPathManager::GetCommonDirectoryPath(E_CommonDirectory CommonDirectory)
    {
        switch (CommonDirectory)
        {
        case E_CommonDirectory::USER_DATA:
            return _UserDataDir;
        case E_CommonDirectory::CACHE:
            return _CacheDir;
        case E_CommonDirectory::EXTERNAL_USER_DATA:
            return _ExternalUserDataDir;
        case E_CommonDirectory::EXTERNAL_CACHE:
            return _ExternalCacheDir;
        }
        return {};
    }

    void F_AndroidPathManager::_QueryDirectories()
    {
        auto& CurrentJNIContext = F_AndroidJNIContext::GetCurrent();
        {
            jclass cls = AndroidJavaItems::PathManager_Bridge::GetClass();
            jmethodID mid = CurrentJNIContext.Env->GetStaticMethodID(cls, "getUserDataDir", "()Ljava/lang/String;");

            jstring jstr = (jstring)CurrentJNIContext.Env->CallStaticObjectMethod(cls, mid);

            const char* chars = CurrentJNIContext.Env->GetStringUTFChars(jstr, nullptr);
            std::string result(chars);
            CurrentJNIContext.Env->ReleaseStringUTFChars(jstr, chars);

            CurrentJNIContext.Env->DeleteLocalRef(jstr);

            _UserDataDir = ToText(result.c_str());
        }
        {
            jclass cls = AndroidJavaItems::PathManager_Bridge::GetClass();
            jmethodID mid = CurrentJNIContext.Env->GetStaticMethodID(cls, "getCacheDir", "()Ljava/lang/String;");

            jstring jstr = (jstring)CurrentJNIContext.Env->CallStaticObjectMethod(cls, mid);

            const char* chars = CurrentJNIContext.Env->GetStringUTFChars(jstr, nullptr);
            std::string result(chars);
            CurrentJNIContext.Env->ReleaseStringUTFChars(jstr, chars);

            CurrentJNIContext.Env->DeleteLocalRef(jstr);

            _CacheDir = ToText(result.c_str());
        }
        {
            jclass cls = AndroidJavaItems::PathManager_Bridge::GetClass();
            jmethodID mid = CurrentJNIContext.Env->GetStaticMethodID(cls, "getExternalUserDataDir", "()Ljava/lang/String;");

            jstring jstr = (jstring)CurrentJNIContext.Env->CallStaticObjectMethod(cls, mid);

            const char* chars = CurrentJNIContext.Env->GetStringUTFChars(jstr, nullptr);
            std::string result(chars);
            CurrentJNIContext.Env->ReleaseStringUTFChars(jstr, chars);

            CurrentJNIContext.Env->DeleteLocalRef(jstr);

            _ExternalUserDataDir = ToText(result.c_str());
        }
        {
            jclass cls = AndroidJavaItems::PathManager_Bridge::GetClass();
            jmethodID mid = CurrentJNIContext.Env->GetStaticMethodID(cls, "getExternalCacheDir", "()Ljava/lang/String;");

            jstring jstr = (jstring)CurrentJNIContext.Env->CallStaticObjectMethod(cls, mid);

            const char* chars = CurrentJNIContext.Env->GetStringUTFChars(jstr, nullptr);
            std::string result(chars);
            CurrentJNIContext.Env->ReleaseStringUTFChars(jstr, chars);

            CurrentJNIContext.Env->DeleteLocalRef(jstr);

            _ExternalCacheDir = ToText(result.c_str());
        }
    }
}
#endif