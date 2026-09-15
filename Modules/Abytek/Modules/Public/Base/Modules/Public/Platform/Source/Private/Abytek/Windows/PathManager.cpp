#include "Abytek/Windows/PathManager.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    F_WindowsPathManager::F_WindowsPathManager()
    {
    }
    F_WindowsPathManager::~F_WindowsPathManager()
    {
    }

    TF_Optional<F_Text> F_WindowsPathManager::GetCommonDirectoryPath(E_CommonDirectory CommonDirectory)
    {
        if (CommonDirectory == E_CommonDirectory::WORKING)
        {
            static constexpr Sz MaxCharCount = 1024;
            WCHAR CWD[MaxCharCount + 1];
            DWORD CharCount = GetCurrentDirectory(MaxCharCount + 1, CWD);
            CWD[CharCount] = 0;
            return F_Text(CWD);
        }
        
        wchar_t buffer[1024];

        DWORD len = GetModuleFileNameW(
            nullptr,       // current executable
            buffer,
            MAX_PATH
        );

        if (len == 0)
            return ABYTEK_TEXT("");

        std::wstring path(buffer, len);

        size_t pos = path.find_last_of(ABYTEK_TEXT("\\/"));

        if (pos == std::wstring::npos)
            return ABYTEK_TEXT("");

        return path.substr(0, pos).c_str();
    }
}
#endif