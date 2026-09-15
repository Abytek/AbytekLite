#include "Abytek/POSIX/FSFile_Standard.hpp"
#include "Abytek/UTF8.hpp"


#ifdef ABYTEK_PLATFORM_POSIX
namespace Abytek
{
    F_POSIXFileDesc::F_POSIXFileDesc(const F_FSFileOpenParams& Desc, int PlatformHandle) :
        A_FSFile(Desc),
        _PlatformHandle(PlatformHandle)
    {
    }
    F_POSIXFileDesc::~F_POSIXFileDesc()
    {
        if(_PlatformHandle != POSIX_INVALID_FILE_PLATFORM_HANDLE)
        {
            close(_PlatformHandle);
        }
    }

    TF_Optional<Sz> F_POSIXFileDesc::GetSize() const
    {
        struct stat FileStat;

        if(fstat(_PlatformHandle, &FileStat) != 0)
        {
            ABYTEK_INTERNAL_FILE_STREAM_LOG_WARNING(_Desc) << "Cannot read file size at \"" << _Desc.Path << "\"";
            return {};
        }

        return static_cast<PDiff>(FileStat.st_size);
    }
    B8 F_POSIXFileDesc::Seek(PDiff NewOffset)
    {
        if(lseek(_PlatformHandle, NewOffset, SEEK_SET) != 0)
        {
            ABYTEK_INTERNAL_FILE_STREAM_LOG_WARNING(_Desc) << "Cannot seek to offset " << NewOffset << " at \"" << _Desc.Path << "\"";
            return false;
        }
        return true;
    }
    TF_Optional<Sz> F_POSIXFileDesc::ReadBinary(const TF_Span<U8>& OutData)
    {
        U8* OutDataBegin = OutData.data();
        PDiff ReadSize = OutData.size();

        if(!ReadSize)
        {
            return 0;
        }

        PDiff InitialOffset = _Offset;
        PDiff OutReadSize;

        if (OutReadSize = read(_PlatformHandle, OutDataBegin, ReadSize); OutReadSize == ssize_t(-1))
        {
            ABYTEK_INTERNAL_FILE_STREAM_LOG_WARNING(_Desc) << "Cannot read binary at offset " << _Offset << " at \"" << _Desc.Path << "\"";
            return {};
        }
        _Offset += OutReadSize;
        return _Offset - InitialOffset;
    }
    TF_Optional<Sz> F_POSIXFileDesc::WriteBinary(const TF_Span<U8>& Data)
    {
        U8* DataBegin = Data.data();
        PDiff WriteSize = Data.size();

        if(!WriteSize)
        {
            return 0;
        }

        PDiff InitialOffset = _Offset;
        PDiff OutWriteSize;

        if (OutWriteSize = write(_PlatformHandle, DataBegin, WriteSize); OutWriteSize == ssize_t(-1))
        {
            ABYTEK_INTERNAL_FILE_STREAM_LOG_WARNING(_Desc) << "Cannot write binary at offset " << _Offset << " at \"" << _Desc.Path << "\"";
            return {};
        }
        _Offset += OutWriteSize;
        return _Offset - InitialOffset;
    }
}
#endif