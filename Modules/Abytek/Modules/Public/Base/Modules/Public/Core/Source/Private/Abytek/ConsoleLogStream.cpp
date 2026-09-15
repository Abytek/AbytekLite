#include "Abytek/ConsoleLogStream.hpp"
#include "Abytek/AndroidLogAdapter.hpp"


namespace Abytek
{
    A_ConsoleLogAdapter::A_ConsoleLogAdapter()
    {
    }
    A_ConsoleLogAdapter::~A_ConsoleLogAdapter()
    {
    }

    void A_ConsoleLogAdapter::Push(F_ConsoleLogStream& LogStream, const F_TextChar* TextChars)
    {
    }

    F_ConsoleLogStream::F_ConsoleLogStream()
    {
#ifdef ABYTEK_PLATFORM_ANDROID
        AddAdapter<F_AndroidLogAdapter>();
#endif
    }
    F_ConsoleLogStream::~F_ConsoleLogStream()
    {
        RemoveAllAdapters();
    }

    void F_ConsoleLogStream::Push(const F_TextChar* TextChars)
    {
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        auto& OutStream = std::wcout;
#else
        auto& OutStream = std::cout;
#endif
        OutStream << TextChars;
        OutStream << ABYTEK_TEXT("\n");

        for (auto Adapter : _Adapters)
        {
            Adapter->Push(*this, TextChars);
        }
    }

    F_ConsoleLogStream ConsoleLogStream;
}