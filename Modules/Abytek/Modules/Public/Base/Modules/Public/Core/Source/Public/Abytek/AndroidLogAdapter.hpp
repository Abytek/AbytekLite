#pragma once

#include "Abytek/ConsoleLogStream.hpp"
#ifdef ABYTEK_PLATFORM_ANDROID
#include <android/log.h>
#endif


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_BASE_CORE_API F_AndroidLogAdapter : public A_ConsoleLogAdapter
    {
    private:

    public:

    public:
        F_AndroidLogAdapter();
        virtual ~F_AndroidLogAdapter() override;
        
    protected:
        virtual void Push(F_ConsoleLogStream& LogStream, const F_TextChar* TextChars) override;
    };
}
#endif