#include "Abytek/AndroidLogAdapter.hpp"
#include "Abytek/Assert.hpp"
#include "Abytek/UTF8.hpp"
#include "Abytek/Log.hpp"
#include "Abytek/CrtUseAllocatorTypedefs.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    F_AndroidLogAdapter::F_AndroidLogAdapter()
    {
    }
    F_AndroidLogAdapter::~F_AndroidLogAdapter()
    {
    }

    void F_AndroidLogAdapter::Push(F_ConsoleLogStream& LogStream, const F_TextChar* TextChars)
    {
        IncreaseLogHiddenCounter();
        
        F_String UTF8String = ToString(TextChars);

        auto LogType = LogStream.GetLogType();
        switch (LogType)
        {
        case E_LogType::LT_INFO:
            __android_log_print(ANDROID_LOG_INFO, "Abytek", "%s", UTF8String.c_str());
            break;
        case E_LogType::LT_VERBOSE:
            __android_log_print(ANDROID_LOG_VERBOSE, "Abytek", "%s", UTF8String.c_str());
            break;
        case E_LogType::LT_VERY_VERBOSE:
            __android_log_print(ANDROID_LOG_VERBOSE, "Abytek", "%s", UTF8String.c_str());
            break;
        case E_LogType::LT_WARNING:
            __android_log_print(ANDROID_LOG_WARN, "Abytek", "%s", UTF8String.c_str());
            break;
        case E_LogType::LT_FATAL:
            __android_log_print(ANDROID_LOG_FATAL, "Abytek", "%s", UTF8String.c_str());
            break;
        case E_LogType::LT_ERROR:
            __android_log_print(ANDROID_LOG_ERROR, "Abytek", "%s", UTF8String.c_str());
            break;
        default:
            assert(false && "Unknown log type");
            break;
        }
        
        DecreaseLogHiddenCounter();
    }
}
#endif