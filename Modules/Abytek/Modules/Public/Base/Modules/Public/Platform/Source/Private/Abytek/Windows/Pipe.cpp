#include "Abytek/Windows/Pipe.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    F_Text ParsePath(const F_Text& RawPath)
    {
        return ABYTEK_TEXT("\\\\.\\pipe\\") + RawPath;
    }

    F_WindowsPipe H_WindowsPipe::Create(const F_PipeDesc& Desc, TS<A_FSFile>& OutFile)
    {
        ABYTEK_BASE_PLATFORM_ASSERT(Desc.Path.length()) << "path cannot be empty";
        
        F_Text ParsedPath = ParsePath(Desc.Path);
        
        F_WindowsPipe Result;

        DWORD DWOpenMode = 0;
        if(Desc.AccessMode == E_PipeAccessMode::BIDIRECTIONAL)
        {
            DWOpenMode = PIPE_ACCESS_DUPLEX;
        }
        if(Desc.AccessMode == E_PipeAccessMode::CLIENT_TO_SERVER)
        {
            DWOpenMode = PIPE_ACCESS_INBOUND;
        }
        if(Desc.AccessMode == E_PipeAccessMode::SERVER_TO_CLIENT)
        {
            DWOpenMode = PIPE_ACCESS_OUTBOUND;
        }

        Result.Internal.Handle = CreateNamedPipe(
            ParsedPath.c_str(),
            DWOpenMode,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            1,
            static_cast<DWORD>(Desc.InBufferSize),
            static_cast<DWORD>(Desc.OutBufferSize),
            NMPWAIT_USE_DEFAULT_WAIT,
            NULL
        );
        if(Result.Internal.Handle == INVALID_HANDLE_VALUE)
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[MAX_PATH];
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                          NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          errorMsg, sizeof(errorMsg), NULL);
            ABYTEK_LOG_WARNING() << "cannot create pipe. " << errorMsg;
            return {};
        }

        F_FSFileOpenParams FileOpenParams;
        FileOpenParams.Path = ParsedPath;
        if(Desc.AccessMode == E_PipeAccessMode::BIDIRECTIONAL)
        {
            FileOpenParams.AccessFlags = FlagCombine(
                FileOpenParams.AccessFlags,
                E_FSFileAccessFlag::READ,
                E_FSFileAccessFlag::WRITE
            );
        }
        if(Desc.AccessMode == E_PipeAccessMode::CLIENT_TO_SERVER)
        {
            FileOpenParams.AccessFlags = FlagCombine(
                FileOpenParams.AccessFlags,
                E_FSFileAccessFlag::READ
            );
        }
        if(Desc.AccessMode == E_PipeAccessMode::SERVER_TO_CLIENT)
        {
            FileOpenParams.AccessFlags = FlagCombine(
                FileOpenParams.AccessFlags,
                E_FSFileAccessFlag::WRITE
            );
        }
        OutFile = TS<F_WindowsFSFile_Standard>()(FileOpenParams, Result.Internal.Handle);

        return Result;
    }
    void H_WindowsPipe::WaitForConnection(const F_WindowsPipe& Pipe)
    {
        ABYTEK_BASE_PLATFORM_ASSERT(Pipe) << "invalid process";
        ConnectNamedPipe(Pipe.Internal.Handle, NULL);
    }
    void H_WindowsPipe::Disconnect(const F_WindowsPipe& Pipe)
    {
        ABYTEK_BASE_PLATFORM_ASSERT(Pipe) << "invalid process";
        DisconnectNamedPipe(Pipe.Internal.Handle);
    }
    void H_WindowsPipe::Destroy(const F_WindowsPipe& Pipe)
    {
        ABYTEK_BASE_PLATFORM_ASSERT(Pipe) << "invalid process";
        CloseHandle(Pipe.Internal.Handle);
    }
    
    TS<A_FSFile> H_WindowsPipe::Client(const F_Text& Path, E_FSFileAccessFlag AccessFlags)
    {
        ABYTEK_BASE_PLATFORM_ASSERT(Path.length()) << "path cannot be empty";
        
        F_Text ParsedPath = ParsePath(Path);
        
        TS<A_FSFile> Result;
        F_FSFileOpenParams FileOpenParams;
        FileOpenParams.Path = ParsedPath;
        FileOpenParams.AccessFlags = AccessFlags;
        if (
            !A_FSAdapter_Standard::GetInstance()->OpenFile(
                FileOpenParams, 
                Result
            )
        )
        {
            return {};
        }
        return Result;
    }
}
#endif