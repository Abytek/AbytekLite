#include "Abytek/POSIX/FSAdapter_Standard.hpp"
#include "Abytek/POSIX/FSFile_Standard.hpp"
#include "Abytek/Path.hpp"


#ifdef ABYTEK_PLATFORM_POSIX
namespace Abytek
{
    F_POSIXFileSystem::F_POSIXFileSystem()
    {
    }
    F_POSIXFileSystem::~F_POSIXFileSystem()
    {
    }

    TS<A_FSFile> F_POSIXFileSystem::OpenFile(const F_FSFileOpenParams& Desc) const
    {
        int OpenMode = 0;
        if(FlagHas(Desc.AccessFlags, E_FSFileAccessFlag::READ))
        {
            OpenMode = O_RDONLY;
        }
        if(FlagHas(Desc.AccessFlags, E_FSFileAccessFlag::WRITE))
        {
            if (FlagHas(Desc.AccessFlags, E_FSFileAccessFlag::READ))
            {
                OpenMode = O_RDWR;
            }
            else
            {
                OpenMode = O_WRONLY;
            }
        }

        ABYTEK_BASE_PLATFORM_ASSERT(Desc.ShareFlags == E_FSFileShareFlag::ALL) << "POSIX only supports E_FSFileShareFlag::ALL.";

        if (Desc.AutoCreate && FlagHas(Desc.AccessFlags, E_FSFileAccessFlag::WRITE))
        {
            OpenMode |= O_CREAT;
        }

        int Handle = open(
            ToString(Desc.Path).c_str(),
            OpenMode,
            0777
        );
        if(Handle == POSIX_INVALID_FILE_PLATFORM_HANDLE)
        {
            ABYTEK_LOG_WARNING() << "Cannot open file \"" << Desc.Path << "\"";
            return {};
        }

        return TS<F_POSIXFileDesc>()(Desc, Handle);
    }
    B8 F_POSIXFileSystem::CreateFile_(const F_Text& Path) const
    {
        
        int Handle;
        mode_t Mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

        Handle = creat(ToString(Path).c_str(), Mode);
        if(Handle == POSIX_INVALID_FILE_PLATFORM_HANDLE)
        {
            ABYTEK_LOG_WARNING() << "Cannot create file \"" << Path << "\"";
            return false;
        }
        close(Handle);
        return true;
    }
    B8 F_POSIXFileSystem::DeleteFile_(const F_Text& Path) const
    {
        
        if (!unlink(ToString(Path).c_str()))
        {
            ABYTEK_LOG_WARNING() << "Cannot delete file \"" << Path << "\"";
            return false;
        }
        return true;
    }
    B8 F_POSIXFileSystem::MoveFile_(const F_Text& NewPath, const F_Text& CurrentPath) const
    {
        
        if (!rename(ToString(CurrentPath).c_str(), ToString(NewPath).c_str()))
        {
            ABYTEK_LOG_WARNING() << "Cannot move file from \"" << CurrentPath << "\" to \"" << NewPath << "\"";
            return false;
        }
        return true;
    }
    B8 F_POSIXFileSystem::CopyFile_(const F_Text& DstPath, const F_Text& SrcPath) const
    {
        
        auto SrcFileDescOpt = OpenFile(
            {
                SrcPath,
                E_FSFileAccessFlag::READ,
                E_FSFileShareFlag::ALL,
                false
            }
        );
        if (!SrcFileDescOpt)
        {
            ABYTEK_LOG_WARNING() << "Cannot copy file from \"" << SrcPath << "\" to \"" << DstPath << "\"";
            return false;
        }

        auto SrcSizeOpt = SrcFileDescOpt->GetSize();
        if (!SrcSizeOpt)
        {
            ABYTEK_LOG_WARNING() << "Cannot copy file from \"" << SrcPath << "\" to \"" << DstPath << "\"";
            return false;
        }

        WithCrtAllocator::TF_Vector<U8> Data(*SrcSizeOpt);
        if (!SrcFileDescOpt->ReadBinary(Data))
        {
            ABYTEK_LOG_WARNING() << "Cannot copy file from \"" << SrcPath << "\" to \"" << DstPath << "\"";
            return false;
        }

        auto DstFileDescOpt = OpenFile(
            {
                DstPath,
                E_FSFileAccessFlag::WRITE,
                E_FSFileShareFlag::ALL,
                true
            }
        );
        if (!DstFileDescOpt)
        {
            ABYTEK_LOG_WARNING() << "Cannot copy file from \"" << SrcPath << "\" to \"" << DstPath << "\"";
            return false;
        }
        if (!DstFileDescOpt->Ate())
        {
            ABYTEK_LOG_WARNING() << "Cannot copy file from \"" << SrcPath << "\" to \"" << DstPath << "\"";
            return false;
        }

        if (!DstFileDescOpt->WriteBinary(Data))
        {
            ABYTEK_LOG_WARNING() << "Cannot write file from \"" << SrcPath << "\" to \"" << DstPath << "\"";
            return false;
        }

        return true;
    }

    B8 F_POSIXFileSystem::IsFileExists(const F_Text& Path) const
    {
        struct stat sb;
        if (stat(ToString(Path).c_str(), &sb) != 0)
        {
            return false;
        }
        return S_ISREG(sb.st_mode);
    }
    B8 F_POSIXFileSystem::IsDirectoryExists(const F_Text& Path) const
    {
        struct stat sb;
        if (stat(ToString(Path).c_str(), &sb) != 0)
        {
            return false;
        }
        return S_ISDIR(sb.st_mode);
    }

    B8 F_POSIXFileSystem::IsFile(const F_Text& Path) const
    {
        F_String PathStr = ToString(Path);
        if (PathStr.empty())
            return false;

        struct stat Info;
        if (stat(PathStr.c_str(), &Info) != 0)
            return false;

        return S_ISREG(Info.st_mode);
    }
    B8 F_POSIXFileSystem::IsDirectory(const F_Text& Path) const
    {
        F_String PathStr = ToString(Path);
        if (PathStr.empty())
            return false;

        struct stat Info;
        if (stat(PathStr.c_str(), &Info) != 0)
            return false;

        return S_ISDIR(Info.st_mode);
    }

    B8 F_POSIXFileSystem::CreateDirectory_(const F_Text& Path) const
    {
        
        auto PathItems = H_Path::Split(Path);
        auto PathItemCount = PathItems.size();

        if(!PathItemCount)
        {
            ABYTEK_LOG_WARNING() << "Cannot create directory due to empty path";
            return false;
        }

        F_Text DirectoryPath = ABYTEK_TEXT("");

        for(I32 i = 0; i < PathItemCount; ++i)
        {
            if(i != 0)
                DirectoryPath += ABYTEK_TEXT("/");

            DirectoryPath += PathItems[i];

            if(!IsDirectoryExists(DirectoryPath))
            {
                if(mkdir(ToString(DirectoryPath).c_str(), 0777) == -1)
                {
                    ABYTEK_LOG_WARNING() << "Cannot create directory \"" << DirectoryPath << "\"";
                    return false;
                }
            }
        }

        return true;
    }

    F_Text F_POSIXFileSystem::GetCurrentWorkingDirectory() const
    {
        char CWD[PATH_MAX];
        char* Code = getcwd(CWD, sizeof(CWD));
        ABYTEK_BASE_PLATFORM_ASSERT(Code) << "cannot get CWD";
        return ToText(Code);
    }

    TF_Optional<TF_Vector<F_Text>> F_POSIXFileSystem::ReadDirectory(const F_Text& Path) const
    {
        TF_Vector<F_Text> Result;

        F_String PathStr = ToString(Path);
        if (PathStr.empty())
        {
            return {};
        }

        DIR* Dir = opendir(PathStr.c_str());
        if (!Dir)
        {
            return {};
        }

        struct dirent* Entry;
        while ((Entry = readdir(Dir)) != nullptr)
        {
            const char* Name = Entry->d_name;

            // Bỏ "." và ".."
            if (Name[0] == '.')
            {
                if (Name[1] == '\0')
                    continue;
                if (Name[1] == '.' && Name[2] == '\0')
                    continue;
            }

            Result.emplace_back(ToText(Name)); // chỉ push tên, không phải full path
        }

        closedir(Dir);
        return Result;
    }
}
#endif