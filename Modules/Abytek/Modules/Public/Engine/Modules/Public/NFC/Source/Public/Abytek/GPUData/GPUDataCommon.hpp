#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"


namespace Abytek
{
    class F_GPUDataComponentPage;
    
    struct F_GPUDataComponentAllocation
    {
        TS<F_GPUDataComponentPage> Page;
        U64 BeginOffsetInBytes = 0;
        U64 EndOffsetInBytes = 0;
    
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return EndOffsetInBytes - BeginOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return Page && GetSize();
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
    };
    
    struct F_GPUDataComponentAddress
    {
        U32 PageIndex = 0;
        U32 OffsetInBytes = 0;
    };
}