#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/FSAdapter.hpp"


#ifdef ABYTEK_PLATFORM_POSIX
namespace Abytek
{
    class F_POSIXFileDesc;

    /**
     * @brief POSIX implementation of the file system interface
     * 
     * Provides file system operations for POSIX-compatible operating systems
     */
    class ABYTEK_BASE_PLATFORM_API F_POSIXFileSystem : public A_FSAdapter
    {
    public:
        /** @brief Constructor */
        F_POSIXFileSystem();
        /** @brief Destructor */
        virtual ~F_POSIXFileSystem() override;

    public:
        virtual TS<A_FSFile> OpenFile(const F_FSFileOpenParams& Desc) const override;
        virtual B8 CreateFile_(const F_Text& Path) const override;
        virtual B8 DeleteFile_(const F_Text& Path) const override;
        virtual B8 MoveFile_(const F_Text& NewPath, const F_Text& CurrentPath) const override;
        virtual B8 CopyFile_(const F_Text& DstPath, const F_Text& SrcPath) const override;
        virtual B8 IsFileExists(const F_Text& Path) const override;
        virtual B8 IsDirectoryExists(const F_Text& Path) const override;
        virtual B8 IsFile(const F_Text& Path) const override;
        virtual B8 IsDirectory(const F_Text& Path) const override;
        virtual B8 CreateDirectory_(const F_Text& Path) const override;
        virtual F_Text GetCurrentWorkingDirectory() const override;
        virtual TF_Optional<TF_Vector<F_Text>> ReadDirectory(const F_Text& Path) const override;
    };
}
#endif