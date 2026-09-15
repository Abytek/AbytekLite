
#include "Abytek/Windows/Memory.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    Sz H_WindowsMemory::GetPageSize()
    {
#ifdef ABYTEK_PLATFORM_WINDOWS
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        return static_cast<Sz>(sysInfo.dwPageSize);
#else
        return static_cast<Sz>((size_t)sysconf(_SC_PAGESIZE));
#endif
    }

    static DWORD ConvertWindowsLLMemoryBlockFlagsectionFlags(E_MemoryAccessFlag Flags)
    {
        if (Flags == E_MemoryAccessFlag::NONE)
            return PAGE_NOACCESS;
        if (Flags == E_MemoryAccessFlag::ALL)
            return PAGE_EXECUTE_READWRITE;
        if (Flags == E_MemoryAccessFlag::EXECUTABLE_READ)
            return PAGE_EXECUTE_READ;
        if (Flags == E_MemoryAccessFlag::READ_WRITE)
            return PAGE_READWRITE;
        if (Flags == E_MemoryAccessFlag::READ)
            return PAGE_READONLY;
        return PAGE_NOACCESS;
    }

    void* H_WindowsMemory::AllocateLLBlock(const F_LLMemoryBlockDesc& Desc)
    {
        Sz PageSize = GetPageSize();
        size_t TotalSize = ((Desc.Size + PageSize - 1) / PageSize) * PageSize;
        
        DWORD FLProtect = ConvertWindowsLLMemoryBlockFlagsectionFlags(Desc.AccessFlags);

        void* Pointer = VirtualAlloc(
            NULL,
            TotalSize,
            MEM_RESERVE | MEM_COMMIT,
            FLProtect
        );
        if(!Pointer)
        {
            ABYTEK_LOG_WARNING() << "cannot allocate low-level memory block";
            return {};
        }

        return Pointer;
    }
    void H_WindowsMemory::DeallocateLLBlock(void* Pointer)
    {
        VirtualFree(Pointer, 0, MEM_RELEASE);
    }

    F_WindowsMappedMemory H_WindowsMemory::CreateMapped(const F_MappedMemoryDesc& Desc)
    {
        F_WindowsMappedMemory Result;

        DWORD FLProtect = ConvertWindowsLLMemoryBlockFlagsectionFlags(Desc.AccessFlags);

        Result.Internal.Handle = CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            FLProtect,
            (DWORD)((Desc.Size >> 32LL) & 0xFFFFFFFFLL),
            (DWORD)(Desc.Size & 0xFFFFFFFFLL),
            Desc.Name->c_str()
        );
        if(Result.Internal.Handle == INVALID_HANDLE_VALUE)
        {
            ABYTEK_LOG_WARNING() << "cannot create mapped memory";
            return {};
        }

        Result.Address = MapViewOfFile(
            Result.Internal.Handle,
            (FlagHas(Desc.AccessFlags, E_MemoryAccessFlag::READ) ? FILE_MAP_READ : 0)
            | (FlagHas(Desc.AccessFlags, E_MemoryAccessFlag::WRITE) ? FILE_MAP_WRITE : 0) 
            | (FlagHas(Desc.AccessFlags, E_MemoryAccessFlag::EXECUTABLE) ? FILE_MAP_EXECUTE : 0),
            0, 0,
            Desc.Size
        );
        if(Result.Address == 0)
        {
            ABYTEK_LOG_WARNING() << "cannot create mapped memory";
            CloseHandle(Result.Internal.Handle);
            return {};
        }
        
        Result.Name = Desc.Name;
        Result.AccessFlags = Desc.AccessFlags;

        return Result;
    }
    F_WindowsMappedMemory H_WindowsMemory::OpenMapped(const F_Name& Name, E_MemoryAccessFlag AccessFlags)
    {
        F_WindowsMappedMemory Result;
        
        DWORD DWDesiredAccess = (
            (FlagHas(AccessFlags, E_MemoryAccessFlag::READ) ? FILE_MAP_READ : 0)
            | (FlagHas(AccessFlags, E_MemoryAccessFlag::WRITE) ? FILE_MAP_WRITE : 0) 
            | (FlagHas(AccessFlags, E_MemoryAccessFlag::EXECUTABLE) ? FILE_MAP_EXECUTE : 0)
        );

        Result.Internal.Handle = OpenFileMapping(
            DWDesiredAccess,
            false,
            Name->c_str()
        );
        if(Result.Internal.Handle == INVALID_HANDLE_VALUE)
        {
            ABYTEK_LOG_WARNING() << "cannot open mapped memory";
            return {};
        }

        Result.Address = MapViewOfFile(
            Result.Internal.Handle,
            DWDesiredAccess,
            0, 0,
            0
        );
        if(Result.Address == 0)
        {
            ABYTEK_LOG_WARNING() << "cannot open mapped memory";
            CloseHandle(Result.Internal.Handle);
            return {};
        }
        
        Result.Name = Name;
        Result.AccessFlags = AccessFlags;

        Result.IsHost = false;

        return Result;
    }
    void H_WindowsMemory::DestroyOrCloseMapped(const F_WindowsMappedMemory& MappedMemory)
    {
        ABYTEK_BASE_PLATFORM_ASSERT(MappedMemory) << "invalid mapped memory";

        CloseHandle(MappedMemory.Internal.Handle);
    }
}
#endif