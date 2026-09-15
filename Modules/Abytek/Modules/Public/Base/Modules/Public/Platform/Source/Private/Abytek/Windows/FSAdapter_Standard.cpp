#include "Abytek/Windows/FSAdapter_Standard.hpp"
#include "Abytek/FSUtilities.hpp"
#include "Abytek/Windows/FSFile_Standard.hpp"
#include "Abytek/Path.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    F_WindowsFSAdapter_Standard::F_WindowsFSAdapter_Standard()
    {
    }
    F_WindowsFSAdapter_Standard::~F_WindowsFSAdapter_Standard()
    {
    }

    F_FeedbackStatus F_WindowsFSAdapter_Standard::GetEntryStat(
        const F_Text& Path,
        F_FSEntryStat& OutStat
    ) const
    {
        OutStat = {};

        DWORD Attributes = GetFileAttributesW(Path.c_str());

        if (Attributes == INVALID_FILE_ATTRIBUTES)
        {
            return F_FeedbackStatus::MakeSucceeded();
        }

        if (Attributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            OutStat.Type = E_FSEntryType::DIRECTORY;
        }
        else
        {
            OutStat.Type = E_FSEntryType::FILE;
        }

        return F_FeedbackStatus::MakeSucceeded();
    }

    F_FeedbackStatus F_WindowsFSAdapter_Standard::OpenFile(const F_FSFileOpenParams& OpenParams, TS<A_FSFile>& OutFile) const
    {
        DWORD DesiredAccess = 0;
        if(FlagHas(OpenParams.AccessFlags, E_FSFileAccessFlag::READ))
        {
            DesiredAccess |= GENERIC_READ;
        }
        if(FlagHas(OpenParams.AccessFlags, E_FSFileAccessFlag::WRITE))
        {
            DesiredAccess |= GENERIC_WRITE;
        }

        DWORD ShareMode = 0;
        if(FlagHas(OpenParams.ShareFlags, E_FSFileShareFlag::DELETE_))
        {
            ShareMode |= FILE_SHARE_DELETE;
        }
        if(FlagHas(OpenParams.ShareFlags, E_FSFileShareFlag::READ))
        {
            ShareMode |= FILE_SHARE_READ;
        }
        if(FlagHas(OpenParams.ShareFlags, E_FSFileShareFlag::WRITE))
        {
            ShareMode |= FILE_SHARE_WRITE;
        }

        DWORD CreationDisposition = OPEN_EXISTING;
        if(OpenParams.AutoCreate && FlagHas(OpenParams.AccessFlags, E_FSFileAccessFlag::WRITE))
        {
            CreationDisposition = OPEN_ALWAYS;
        }
        
        HANDLE Handle = CreateFile(
            OpenParams.Path.c_str(),
            DesiredAccess,
            ShareMode,
            0,
            CreationDisposition,
            FILE_ATTRIBUTE_NORMAL,
            0
        );
        if(Handle == INVALID_HANDLE_VALUE)
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot open file \"")  
                + OpenParams.Path
                + ABYTEK_TEXT("\"")
            );
        }

        OutFile = TS<F_WindowsFSFile_Standard>()(OpenParams, Handle);
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSAdapter_Standard::CreateFile_(const F_Text& Path) const
    {
        DWORD DesiredAccess = 0;
        DWORD ShareMode = 0;
        DWORD CreationDisposition = CREATE_NEW;
        
        HANDLE Handle = CreateFile(
            Path.c_str(),
            DesiredAccess,
            ShareMode,
            0,
            CreationDisposition,
            FILE_ATTRIBUTE_NORMAL,
            0
        );
        if(Handle == INVALID_HANDLE_VALUE)
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot create file \"")
                + Path 
                + ABYTEK_TEXT("\"")
            );
        }
        
        CloseHandle(Handle);
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSAdapter_Standard::DeleteFile_(const F_Text& Path) const
    {
        if (!DeleteFile(Path.c_str()))
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot delete file \"")
                + Path 
                + ABYTEK_TEXT("\"")
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSAdapter_Standard::MoveFile_(const F_Text& NewPath, const F_Text& CurrentPath) const
    {
        if (!MoveFileExW(CurrentPath.c_str(), NewPath.c_str(), MOVEFILE_REPLACE_EXISTING))
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot move file from \"")
                + CurrentPath 
                + ABYTEK_TEXT("\" to \"") 
                + NewPath 
                + ABYTEK_TEXT("\"")
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSAdapter_Standard::CopyFile_(const F_Text& DstPath, const F_Text& SrcPath) const
    {
        if (!CopyFile(SrcPath.c_str(), DstPath.c_str(), false))
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot copy file from \"")
                + SrcPath 
                + ABYTEK_TEXT("\" to \"") 
                + DstPath 
                + ABYTEK_TEXT("\"")
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSAdapter_Standard::CreateDirectory_(const F_Text& Path) const
    {    
        auto PathItems = H_Path::Split(Path);
        auto PathItemCount = PathItems.size();

        if(!PathItemCount)
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot create directory due to empty path")    
            );
        }

        F_Text DirectoryPath = ABYTEK_TEXT("");

        for(I32 Idx = 0; Idx < PathItemCount; ++Idx)
        {
            if(Idx != 0)
            {
                DirectoryPath += ABYTEK_TEXT("/");
            }

            DirectoryPath += PathItems[Idx];

            B8 ShouldCreateDirectory = true;
            
            F_FSEntryStat Stat;
            ABYTEK_FEEDBACK_STATUS_CHECK(GetEntryStat(DirectoryPath, Stat));
            if (Stat.Type != E_FSEntryType::NONE)
            {
                if (Stat.Type != E_FSEntryType::DIRECTORY)
                {
                    return F_FeedbackStatus::MakeFailed(
                        ABYTEK_TEXT("\"")
                        + DirectoryPath 
                        + ABYTEK_TEXT("\" is not a directory")
                    );
                }
                ShouldCreateDirectory = false;
            }
            
            if(ShouldCreateDirectory)
            {
                if(!CreateDirectory(DirectoryPath.c_str(), 0))
                {
                    return F_FeedbackStatus::MakeFailed(
                        ABYTEK_TEXT("Cannot create directory \"")
                        + DirectoryPath 
                        + ABYTEK_TEXT("\"")
                    );
                }
            }
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSAdapter_Standard::DeleteDirectory_(const F_Text& Path) const
    {
        {
            TF_Vector<F_Text> EntryRelativePaths;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                H_FSUtilities::ListEntries<E_FSEntryType::FILE>(Path, EntryRelativePaths)  
            );
            for (const auto& Entry : EntryRelativePaths)
            {
                F_Text EntryAbsolutePath = Path;
                if (EntryAbsolutePath.size() > 0)
                {
                    if (
                        (EntryAbsolutePath.back() != ABYTEK_TEXT('/'))
                        && (EntryAbsolutePath.back() != ABYTEK_TEXT('\\'))
                    )
                    {
                        EntryAbsolutePath += ABYTEK_TEXT('/');
                    }
                }
                EntryAbsolutePath += Entry;
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    DeleteFile_(EntryAbsolutePath)  
                );
            }
        }
        {
            TF_Vector<F_Text> EntryRelativePaths;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                H_FSUtilities::ListEntries<E_FSEntryType::DIRECTORY>(Path, EntryRelativePaths)  
            );
            for (const auto& Entry : EntryRelativePaths)
            {
                F_Text EntryAbsolutePath = Path;
                if (EntryAbsolutePath.size() > 0)
                {
                    if (
                        (EntryAbsolutePath.back() != ABYTEK_TEXT('/'))
                        && (EntryAbsolutePath.back() != ABYTEK_TEXT('\\'))
                    )
                    {
                        EntryAbsolutePath += ABYTEK_TEXT('/');
                    }
                }
                EntryAbsolutePath += Entry;
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    DeleteDirectory_(EntryAbsolutePath)  
                );
            }
        }
        if (!RemoveDirectoryW(Path.c_str()))
        {
            DWORD err = GetLastError();

            wchar_t* msg = nullptr;
            FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                err,
                0,
                (wchar_t*)&msg,
                0,
                nullptr
            );
            F_Text Msg = ToText(msg);
            LocalFree(msg);
            
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot delete directory \"")
                + Path
                + ABYTEK_TEXT("\": ") + Msg
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSAdapter_Standard::MoveDirectory_(const F_Text& NewPath, const F_Text& CurrentPath) const
    {
        if (!MoveFileW(CurrentPath.c_str(), NewPath.c_str()))
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Cannot move directory from \"")
                + CurrentPath
                + ABYTEK_TEXT("\" to \"")
                + NewPath
                + ABYTEK_TEXT("\"")
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSAdapter_Standard::CopyDirectory_(const F_Text& DstPath, const F_Text& SrcPath) const
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(CreateDirectory_(DstPath));

        TF_Vector<F_Text> Entries;
        ABYTEK_FEEDBACK_STATUS_CHECK(ReadDirectory(SrcPath, Entries));

        for (const auto& Entry : Entries)
        {
            F_Text SrcEntry = SrcPath + ABYTEK_TEXT("/") + Entry;
            F_Text DstEntry = DstPath + ABYTEK_TEXT("/") + Entry;

            F_FSEntryStat Stat;
            ABYTEK_FEEDBACK_STATUS_CHECK(GetEntryStat(SrcEntry, Stat));

            switch (Stat.Type)
            {
            case E_FSEntryType::FILE:
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    CopyFile_(DstEntry, SrcEntry)
                );
                break;

            case E_FSEntryType::DIRECTORY:
                ABYTEK_FEEDBACK_STATUS_CHECK(
                    CopyDirectory_(DstEntry, SrcEntry)
                );
                break;

            default:
                break;
            }
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsFSAdapter_Standard::ReadDirectory(const F_Text& Path, TF_Vector<F_Text>& OutEntries) const
    {    // Ensure path ends with backslash or wildcard appended
        F_Text searchPath = Path;
        if (searchPath.empty())
        {
            // throw std::invalid_argument("dirPath is empty");
            return F_FeedbackStatus::MakeFailed();
        }
        wchar_t last = searchPath.back();
        if (last != ABYTEK_TEXT('\\') && last != ABYTEK_TEXT('/')) searchPath += ABYTEK_TEXT('\\');
        searchPath += ABYTEK_TEXT("*");

        TF_Vector<F_Text> items;
        WIN32_FIND_DATAW fd;
        HANDLE hFind = FindFirstFileW(searchPath.c_str(), &fd);
        if (hFind == INVALID_HANDLE_VALUE) {
            // DWORD err = GetLastError();
            // If directory doesn't exist or access denied, throw with code
            // throw std::runtime_error("FindFirstFileW failed, error code: " + std::to_string(err));
            return F_FeedbackStatus::MakeFailed();
        }

        do {
            const F_Text name = fd.cFileName;
            if (name != ABYTEK_TEXT(".") && name != ABYTEK_TEXT("..")) {
                items.push_back(name);
            }
        } while (FindNextFileW(hFind, &fd) != 0);

        DWORD finalErr = GetLastError();
        FindClose(hFind);
        if (finalErr != ERROR_NO_MORE_FILES) {
            //throw std::runtime_error("FindNextFileW failed, error code: " + std::to_string(finalErr));
            return {};
        }

        OutEntries = ABYTEK_MOVE(items);
        return F_FeedbackStatus::MakeSucceeded();
    }
}
#endif