#pragma once

#include "Abytek/FSFile_Standard.hpp"


#ifdef ABYTEK_PLATFORM_POSIX
namespace Abytek
{
    /**
     * @brief Invalid file handle constant for POSIX systems
     * 
     * Represents an invalid file descriptor on POSIX platforms
     */
    static constexpr int POSIX_INVALID_FILE_PLATFORM_HANDLE = ~0;


    /**
     * @brief POSIX-specific file stream implementation
     * 
     * Provides file operations for POSIX-based platforms using file descriptors
     */
    class ABYTEK_BASE_PLATFORM_API F_POSIXFileDesc final : public A_FSFile
    {
    public:
        friend class A_FileSystem;
        friend class F_POSIXFileSystem;

    private:
        int _PlatformHandle = POSIX_INVALID_FILE_PLATFORM_HANDLE;  ///< POSIX file descriptor

    public:
        /**
         * @brief Gets the underlying POSIX file descriptor
         * @return The file descriptor
         */
        ABYTEK_FORCE_INLINE int GetPlatformHandle() const noexcept { return _PlatformHandle; }

    public:
        F_POSIXFileDesc(const F_FSFileOpenParams& Desc, int PlatformHandle);
        virtual ~F_POSIXFileDesc() override;

    public:
        virtual TF_Optional<Sz> GetSize() const override;
        virtual B8 Seek(PDiff NewOffset) override;
        virtual TF_Optional<Sz> ReadBinary(const TF_Span<U8>& OutData) override;
        virtual TF_Optional<Sz> WriteBinary(const TF_Span<U8>& Data) override;
    };
}
#endif