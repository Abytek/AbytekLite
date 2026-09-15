#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/Path.hpp"


namespace Abytek
{
    enum class E_FSFileAccessFlag
    {
        NONE = 0x0,
        READ = 0x1,
        WRITE = 0x2,

        ALL = READ | WRITE,
        DEFAULT = ALL
    };
    enum class E_FSFileShareFlag
    {
        NONE = 0x0,
        DELETE_ = 0x1,
        READ = 0x2,
        WRITE = 0x4,

        ALL = DELETE_ | READ | WRITE,
        DEFAULT = ALL
    };
    struct F_FSFileOpenParams
    {
        F_Text Path;
        E_FSFileAccessFlag AccessFlags = E_FSFileAccessFlag::DEFAULT;
        E_FSFileShareFlag ShareFlags = E_FSFileShareFlag::DEFAULT;
        B8 AutoCreate = true;
    };
    
    enum class E_FSEntryType : U8
    {
        NONE = 0x0,
        FILE = 0x1,
        DIRECTORY = 0x2,
        
        ALL = FILE | DIRECTORY
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_FSEntryType);
    struct F_FSEntryStat
    {
        E_FSEntryType Type = E_FSEntryType::NONE;
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return Type != E_FSEntryType::NONE;
        }
        ABYTEK_FORCE_INLINE operator B8 () const noexcept
        {
            return IsValid();
        }
    };
}