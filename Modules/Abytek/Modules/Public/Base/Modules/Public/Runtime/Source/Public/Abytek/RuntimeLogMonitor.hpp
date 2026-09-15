#pragma once

#include "Abytek/Base.Runtime.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_BASE_RUNTIME_API F_RuntimeLogMonitor final : public A_Object
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_RuntimeLogMonitor);

    public:
        class F_TeeBufferWide : public std::wstreambuf
        {
        public:
            F_TeeBufferWide(const TS_Valid<A_FSFile>& InFileDesc) :
                Original(std::wcout.rdbuf()),
                FileDesc(InFileDesc)
            {}

        protected:
            traits_type::int_type overflow(traits_type::int_type c) override
            {
                if (c != traits_type::eof())
                {
                    Original->sputc((wchar_t)c);     // write to console
                    Original->pubsync();
                    F_Text Text = ToText(c);
                    FileDesc->WriteText(Text);       // save to string
#ifdef ABYTEK_PLATFORM_WINDOWS
                    wchar_t buf[2]{ (wchar_t)c, 0 };
                    OutputDebugStringW(buf);
#endif
                } 
                return traits_type::not_eof(c); 
            }
            std::streamsize xsputn(const wchar_t* s, std::streamsize n) override
            {
                Original->sputn(s, n);
                Original->pubsync();
                F_Text Data;
                Data.resize(n);
                memcpy(Data.data(), s, sizeof(wchar_t) * n);
                FileDesc->WriteText(Data);
#ifdef ABYTEK_PLATFORM_WINDOWS
                OutputDebugStringW(Data.c_str());
#endif
                return n;
            }

        private:
            std::wstreambuf* Original;
            TS<A_FSFile> FileDesc;

        public:
            auto GetOriginal() const noexcept
            {
                return Original;
            }
        };

        class F_TeeBuffer : public std::streambuf
        {
        public:
            F_TeeBuffer(const TS_Valid<A_FSFile>& InFileDesc) :
                Original(std::cout.rdbuf()),
                FileDesc(InFileDesc)
            {}

        protected:
            traits_type::int_type overflow(traits_type::int_type c) override
            {
                if (c != traits_type::eof())
                {
                    Original->sputc(c);     // write to console
                    Original->pubsync();
                    F_String String = ToString(c);
                    FileDesc->WriteString(String);       // save to string
#ifdef ABYTEK_PLATFORM_WINDOWS
                    char buf[2]{ (char)c, 0 };
                    OutputDebugStringA(buf);
#endif
                } 
                return traits_type::not_eof(c); 
            }
            std::streamsize xsputn(const char* s, std::streamsize n) override
            {
                Original->sputn(s, n);
                Original->pubsync();
                F_String Data;
                Data.resize(n);
                memcpy(Data.data(), s, sizeof(F_Char) * n);
                FileDesc->WriteString(Data);
#ifdef ABYTEK_PLATFORM_WINDOWS
                OutputDebugStringA(Data.c_str());
#endif
                return n;
            }

        private:
            std::streambuf* Original;
            TS<A_FSFile> FileDesc;

        public:
            auto GetOriginal() const noexcept
            {
                return Original;
            }
        };
        
        
    private:
        TS<A_FSFile> _File;

        TU_Unmanaged<F_TeeBufferWide> _TeeBufferWide;
        TU_Unmanaged<F_TeeBuffer> _TeeBuffer;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetFileDesc() const noexcept
        {
            return _File;
        }
        ABYTEK_FORCE_INLINE auto& GetFileDesc() noexcept
        {
            return _File;
        }
        
    public:
        F_RuntimeLogMonitor();
        ~F_RuntimeLogMonitor() override;
    };
}
