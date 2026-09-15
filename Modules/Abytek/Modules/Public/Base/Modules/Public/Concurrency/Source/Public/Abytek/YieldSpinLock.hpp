#pragma once

#include "Abytek/Base.Concurrency.prerequisites.pch.hpp"


namespace Abytek
{
    struct F_YieldSpinLock
    {
        F_AtomicFlag IsLocked;
        
        ABYTEK_FORCE_INLINE B8 TryLock()
        {
            return !IsLocked.test_and_set(boost::memory_order_acquire);
        }
        ABYTEK_FORCE_INLINE void Lock()
        {
            while (!TryLock())
            {
                H_TaskUtilities::Switch();
            }
        }
        ABYTEK_FORCE_INLINE void Lock(E_TaskPriority TaskPriority)
        {
            while (!TryLock())
            {
                H_TaskUtilities::ChangePriority(TaskPriority);
                H_TaskUtilities::Switch();
            }
        }
        ABYTEK_FORCE_INLINE void Unlock()
        {
            ABYTEK_BASE_CORE_ASSERT(IsLocked.test(boost::memory_order_acquire));
            IsLocked.clear(boost::memory_order_release);
        }
    };
}