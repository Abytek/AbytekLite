#pragma once

#include "Abytek/Base.Concurrency.prerequisites.pch.hpp"
#include "Abytek/YieldSpinLock.hpp"


namespace Abytek
{
    struct F_YieldCriticalSection
    {
        mutable F_YieldSpinLock Lock;
        
        ABYTEK_FORCE_INLINE void Begin() const
        {
            Lock.Lock();
        }
        ABYTEK_FORCE_INLINE void End() const
        {
            Lock.Unlock();
        }
        
        template<typename __F_Callback>
        ABYTEK_FORCE_INLINE void operator () (__F_Callback&& Callback) const
        {
            Begin();
            Callback();
            End();
        }
        
        template<typename __F_Callback>
        ABYTEK_FORCE_INLINE void Share(__F_Callback&& Callback) const
        {
            Lock.Unlock();
            Callback();
            Lock.Lock();
        }
    };
}