#pragma once

#include "Abytek/Base.Concurrency.prerequisites.pch.hpp"
#include "Abytek/YieldSpinLock.hpp"


namespace Abytek
{
    struct F_YieldReentrantCriticalSection
    {
        mutable boost::atomic<F_TaskId> TaskId;
        mutable F_YieldSpinLock Lock;
        
        struct F_Scope
        {
            B8 ShouldDeferUnlock = false;
        };
        ABYTEK_FORCE_INLINE F_Scope Begin() const
        {
            F_Scope Result;
            auto CurrentTaskId = H_TaskUtilities::GetId();
            while (!Lock.TryLock())
            {
                if (TaskId == CurrentTaskId)
                {
                    break;
                }
                H_TaskUtilities::Switch();
            }
            if (TaskId != CurrentTaskId)
            {
                TaskId = CurrentTaskId;
                Result.ShouldDeferUnlock = true;
            }
            return Result;
        }
        ABYTEK_FORCE_INLINE void End(const F_Scope& Scope) const
        {
            if (Scope.ShouldDeferUnlock)
            {
                TaskId = INVALID_TASK_ID;
                Lock.Unlock();
            }
        }
        
        template<typename __F_Callback>
        ABYTEK_FORCE_INLINE void operator () (__F_Callback&& Callback) const
        {
            F_Scope Scope = Begin();
            Callback();
            End(Scope);
        }
        
        template<typename __F_Callback>
        ABYTEK_FORCE_INLINE void Share(__F_Callback&& Callback) const
        {
            (*this)(
                [this, &Callback]
                {
                    Lock.Unlock();
                    Callback();
                    H_TaskUtilities::Switch();
                    Lock.Lock();
                }
            );
        }
    };
}