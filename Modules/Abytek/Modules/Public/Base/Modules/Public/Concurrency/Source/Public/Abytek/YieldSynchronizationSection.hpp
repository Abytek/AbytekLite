#pragma once

#include "Abytek/Base.Concurrency.prerequisites.pch.hpp"
#include "Abytek/YieldSpinLock.hpp"


namespace Abytek
{
    struct F_YieldSynchronizationSection
    {
        mutable F_SpinLock Lock;
        mutable F_AtomicFlag Flag;
        mutable AU32 Counter;
        
        ABYTEK_FORCE_INLINE void Begin() const
        {
            Flag.test_and_set(boost::memory_order_release);
        }
        ABYTEK_FORCE_INLINE void End() const
        {
            while (true)
            {
                {
                    TF_ScopeLock<F_SpinLock> _(Lock);
                    if (Counter.load(boost::memory_order_acquire) == 0)
                    {
                        Flag.clear(boost::memory_order_release);
                        break;
                    }
                }
                H_TaskUtilities::Switch();
            }
        }
        template<typename __F_Callback>
        void Ensure(__F_Callback&& Callback) const
        {
            while (true)
            {
                {
                    TF_ScopeLock<F_SpinLock> _(Lock);
                    if (Flag.test(boost::memory_order_acquire))
                    {
                        Counter.fetch_add(1, boost::memory_order_release);
                        break;
                    }
                }
                H_TaskUtilities::Switch();
            }
            Callback();
            Counter.fetch_sub(1, boost::memory_order_release);
        }
    };
}