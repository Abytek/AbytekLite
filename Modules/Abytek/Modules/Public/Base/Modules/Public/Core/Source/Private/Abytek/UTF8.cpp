#include "Abytek/UTF8.hpp"

#ifdef ABYTEK_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#ifdef ABYTEK_PLATFORM_POSIX
#include <iconv.h>
#include <errno.h>
#include <cstring>
#include <vector>
#endif

namespace Abytek
{
    boost::container::vector<F_Char> H_UTF8::From(
        const F_TextChar& TextChar)
    {
        boost::container::vector<F_TextChar> Text;
        Text.push_back(TextChar);

        return From(Text);
    }

    boost::container::vector<F_Char> H_UTF8::From(
        const boost::container::vector<F_TextChar>& Text)
    {
        if (Text.empty())
        {
            return {};
        }

#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT

        boost::container::vector<F_Char> Result;

#ifdef ABYTEK_PLATFORM_WINDOWS

        int NumBytes = WideCharToMultiByte(
            CP_UTF8,
            0,
            Text.data(),
            static_cast<int>(Text.size()),
            nullptr,
            0,
            nullptr,
            nullptr
        );

        if (NumBytes <= 0)
        {
            return {};
        }

        Result.resize(NumBytes);

        WideCharToMultiByte(
            CP_UTF8,
            0,
            Text.data(),
            static_cast<int>(Text.size()),
            Result.data(),
            NumBytes,
            nullptr,
            nullptr
        );

#elif ABYTEK_PLATFORM_POSIX

        iconv_t cd = iconv_open(
            "UTF-8",
            sizeof(F_TextChar) == 4 ?
                "UTF-32LE" :
                "UTF-16LE"
        );

        if (cd == (iconv_t)-1)
        {
            return {};
        }

        char* inBuf =
            reinterpret_cast<char*>(
                const_cast<F_TextChar*>(Text.data())
            );

        size_t inBytesLeft =
            Text.size() * sizeof(F_TextChar);

        // worst case UTF8 = 4 bytes/codepoint
        size_t outCapacity =
            Text.size() * 4;

        Result.resize(outCapacity);

        char* outBuf = Result.data();
        size_t outBytesLeft = outCapacity;

        if (
            iconv(
                cd,
                &inBuf,
                &inBytesLeft,
                &outBuf,
                &outBytesLeft
            ) == (size_t)-1
        )
        {
            iconv_close(cd);
            return {};
        }

        iconv_close(cd);

        Result.resize(
            outCapacity - outBytesLeft
        );

#else
#error "Unknown platform"
#endif

        return Result;

#else
        return Text;
#endif
    }


    boost::container::vector<F_TextChar> H_UTF8::ToText(
        const F_Char& Char)
    {
        boost::container::vector<F_Char> String;
        String.push_back(Char);

        return ToText(String);
    }

    boost::container::vector<F_TextChar> H_UTF8::ToText(
        const boost::container::vector<F_Char>& String)
    {
        if (String.empty())
        {
            return {};
        }

#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT

        boost::container::vector<F_TextChar> Result;

#ifdef ABYTEK_PLATFORM_WINDOWS

        int NumChars = MultiByteToWideChar(
            CP_UTF8,
            0,
            String.data(),
            static_cast<int>(String.size()),
            nullptr,
            0
        );

        if (NumChars <= 0)
        {
            return {};
        }

        Result.resize(NumChars);

        MultiByteToWideChar(
            CP_UTF8,
            0,
            String.data(),
            static_cast<int>(String.size()),
            Result.data(),
            NumChars
        );

#elif ABYTEK_PLATFORM_POSIX

        iconv_t cd = iconv_open(
            sizeof(F_TextChar) == 4 ?
                "UTF-32LE" :
                "UTF-16LE",
            "UTF-8"
        );

        if (cd == (iconv_t)-1)
        {
            return {};
        }

        char* inBuf =
            const_cast<char*>(String.data());

        size_t inBytesLeft =
            String.size();

        // worst case:
        // UTF8 byte -> 1 wchar
        size_t outCapacity =
            String.size() *
            sizeof(F_TextChar);

        Result.resize(
            outCapacity /
            sizeof(F_TextChar)
        );

        char* outBuf =
            reinterpret_cast<char*>(
                Result.data()
            );

        size_t outBytesLeft =
            outCapacity;

        if (
            iconv(
                cd,
                &inBuf,
                &inBytesLeft,
                &outBuf,
                &outBytesLeft
            ) == (size_t)-1
        )
        {
            iconv_close(cd);
            return {};
        }

        iconv_close(cd);

        size_t convertedBytes =
            outCapacity -
            outBytesLeft;

        Result.resize(
            convertedBytes /
            sizeof(F_TextChar)
        );

#else
#error "Unknown platform"
#endif

        return Result;

#else
        return String;
#endif
    }
}
