#include "Abytek/RuntimeLogMonitor.hpp"


namespace Abytek
{
    namespace Internal
    {
        std::string GenerateLogFileName(const std::string& prefix = "")
        {
            // Get current time
            std::time_t now = std::time(nullptr);
            std::tm localTime{};

#ifdef _WIN32
            localtime_s(&localTime, &now);   // Windows
#else
            localtime_r(&now, &localTime);   // Linux/macOS
#endif

            std::ostringstream oss;

            oss << prefix
                << std::put_time(&localTime, "%Y-%m-%d_%H-%M-%S")
                << ".log";

            return oss.str();
        }
    }

    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_RuntimeLogMonitor);
    
    F_RuntimeLogMonitor::F_RuntimeLogMonitor()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
        
        F_Text LogDirectoryPath = (
            *A_PathManager::GetInstance()->GetCommonDirectoryPath(
                E_CommonDirectory::CACHE
            )
            + ABYTEK_TEXT("/Abytek/Logs")
        );
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            H_FSUtilities::EnsureDirectory(LogDirectoryPath)
        );
        
        F_FSFileOpenParams FileOpenParams;
        FileOpenParams.Path = LogDirectoryPath + ABYTEK_TEXT("/") + ToText(Internal::GenerateLogFileName().c_str());
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            A_FSAdapter_Standard::GetInstance()->OpenFile(FileOpenParams, _File)
        );

        _TeeBufferWide = TU_Unmanaged<F_TeeBufferWide>()(_File);
        _TeeBuffer = TU_Unmanaged<F_TeeBuffer>()(_File);

        std::wcout.rdbuf(_TeeBufferWide.GetObjectRawP());
        std::cout.rdbuf(_TeeBuffer.GetObjectRawP());
    }
    F_RuntimeLogMonitor::~F_RuntimeLogMonitor()
    {
        std::wcout.rdbuf(_TeeBufferWide->GetOriginal());
        std::cout.rdbuf(_TeeBuffer->GetOriginal());
    }
}