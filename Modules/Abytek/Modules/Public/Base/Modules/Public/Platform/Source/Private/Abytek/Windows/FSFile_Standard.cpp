#include "Abytek/Windows/FSFile_Standard.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    F_WindowsFSFile_Standard::F_WindowsFSFile_Standard(const F_FSFileOpenParams& OpenParams, HANDLE PlatformHandle) :
        A_FSFile_Standard(OpenParams),
        _PlatformHandle(PlatformHandle)
    {
    }
    F_WindowsFSFile_Standard::~F_WindowsFSFile_Standard()
    {
        if(_PlatformHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(_PlatformHandle);
        }
    }
    F_FeedbackStatus F_WindowsFSFile_Standard::SetSize(Sz NewSize) const
    {
        LARGE_INTEGER Position;
        Position.QuadPart = static_cast<LONGLONG>(NewSize);

        if (!SetFilePointerEx(_PlatformHandle, Position, nullptr, FILE_BEGIN))
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot seek to new file size at \"")
                + GetPath()
                + ABYTEK_TEXT("\"")
            );
        }

        if (!SetEndOfFile(_PlatformHandle))
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot set file size at \"")
                + GetPath()
                + ABYTEK_TEXT("\"")
            );
        }

        return F_FeedbackStatus::MakeSucceeded();
    }
    
    F_FeedbackStatus F_WindowsFSFile_Standard::GetSize(Sz& OutSize) const
    {
        LARGE_INTEGER FileSize;

        if (!GetFileSizeEx(_PlatformHandle, &FileSize))
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot read file size at \"")
                + GetPath()
                + ABYTEK_TEXT("\"")
            );
        }

        OutSize = static_cast<Sz>(FileSize.QuadPart);
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSFile_Standard::Seek(PDiff NewOffset)
    {
        LARGE_INTEGER Li;
        Li.QuadPart = NewOffset;
        if(!SetFilePointerEx(_PlatformHandle, Li, nullptr, FILE_BEGIN))
        {
            DWORD err = GetLastError();
            TCHAR errorMsg[MAX_PATH];
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                          NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          errorMsg, sizeof(errorMsg), NULL);
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot seek to offset ") 
                + ToText(NewOffset) 
                + ABYTEK_TEXT(" at \"") 
                + GetPath() 
                + ABYTEK_TEXT("\": ")
                + ToText(errorMsg)
            );
        }
        _Offset = NewOffset;
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSFile_Standard::ReadBinary(const TF_Span<U8>& OutData, Sz& OutSize)
    {
        U8* OutDataBegin = OutData.data();
        PDiff ReadSize = OutData.size();

        if(!ReadSize)
        {
            OutSize = 0;
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        PDiff InitialOffset = _Offset;

        PDiff BeginOffset = 0;
        PDiff EndOffset = 0;

        while (ReadSize > 0)
        {
            BeginOffset = EndOffset;
            EndOffset = BeginOffset + Min(ReadSize, static_cast<PDiff>(ABYTEK_U32_MAX));

            DWORD OutReadSize;
            if(
                !ReadFile(
                    _PlatformHandle,
                    OutDataBegin + BeginOffset,
                    static_cast<DWORD>(EndOffset - BeginOffset),
                    &OutReadSize,
                    0
                )
            )
            {
                return F_FeedbackStatus::MakeFailed(
                    ABYTEK_TEXT("Cannot read binary at offset ")
                    + ToText(_Offset) 
                    + ABYTEK_TEXT(" at \"") 
                    + GetPath() 
                    + ABYTEK_TEXT("\"")
                );
            }

            _Offset += static_cast<PDiff>(OutReadSize);
            if (OutReadSize == 0)
            {
                break;
            }
            ReadSize -= OutReadSize;
        }   
        OutSize = _Offset - InitialOffset;
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSFile_Standard::WriteBinary(const TF_Span<const U8>& Data, Sz& OutSize)
    {
        const U8* DataBegin = Data.data();
        PDiff WriteSize = Data.size();

        if(!WriteSize)
        {
            OutSize = 0;
            return F_FeedbackStatus::MakeSucceeded();
        }

        PDiff InitialOffset = _Offset;

        PDiff BeginOffset = 0;
        PDiff EndOffset = 0;

        while (WriteSize > 0)
        {
            BeginOffset = EndOffset;
            EndOffset = BeginOffset + Min(WriteSize, static_cast<PDiff>(ABYTEK_U32_MAX));

            DWORD OutWriteSize;
            if(
                !WriteFile(
                    _PlatformHandle,
                    DataBegin + BeginOffset,
                    static_cast<DWORD>(EndOffset - BeginOffset),
                    &OutWriteSize,
                    0
                )
            )
            {
                return F_FeedbackStatus::MakeFailed(
                    ABYTEK_TEXT("Cannot write binary at offset ")
                    + ToText(_Offset) 
                    + ABYTEK_TEXT(" at \"") 
                    + GetPath() 
                    + ABYTEK_TEXT("\"")
                );
            }

            _Offset += static_cast<PDiff>(OutWriteSize);
            if (OutWriteSize == 0)
            {
                break;
            }
            WriteSize -= OutWriteSize;
        }   
        OutSize = _Offset - InitialOffset;
        return F_FeedbackStatus::MakeSucceeded();
    }
}
#endif