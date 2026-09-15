#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"
#include "Abytek/TaskPromise.hpp"


namespace Abytek
{
    class F_TaskContext;
    
    using F_TaskId = U64;
    static constexpr F_TaskId INVALID_TASK_ID = ~F_TaskId(0);
    
    using F_TaskFunctor = TF_Function<void()>;
    enum class E_TaskPriority : U8
    {
        LOW = 0,
        MEDIUM,
        HIGH,
        EXTREME,

        NUM,

        DEFAULT = MEDIUM,
    };

    constexpr U32 ConvertTaskPriorityToNumInstanceSetsPerCycle(E_TaskPriority Priority)
    {
        if (Priority == E_TaskPriority::LOW)
        {
            return 1;
        }
        if (Priority == E_TaskPriority::MEDIUM)
        {
            return 2;
        }
        if (Priority == E_TaskPriority::HIGH)
        {
            return 4;
        }
        if (Priority == E_TaskPriority::EXTREME)
        {
            return 8;
        }
        return 1;
    }
    
    enum class E_TaskStackSize : U8
    {
        LOW, // 64KB
        MEDIUM, // 128KB
        HIGH, // 256KB
        EXTREME, // 512KB,
        NUM,
        DEFAULT = MEDIUM
    };
    using F_TaskStackSizeIndex = TF_UInt<sizeof(E_TaskStackSize)>;
    inline Sz ResolveTaskStackSize(E_TaskStackSize X)
    {
        switch (X)
        {
        case E_TaskStackSize::LOW:
            return 64 * 1024;
        case E_TaskStackSize::MEDIUM:
            return 128 * 1024;
        case E_TaskStackSize::HIGH:
            return 256 * 1024;
        case E_TaskStackSize::EXTREME:
            return 512 * 1024;
        default:
            ABYTEK_LOG_FATAL() << "Invalid task stack size: " << static_cast<F_TaskStackSizeIndex>(X);
        }
        return 0;
    }
    
    struct ABYTEK_BASE_TASK_API F_Task
    {
        F_TaskFunctor Functor;
        E_TaskPriority Priority = E_TaskPriority::DEFAULT;
        F_Name Name;
        TS_Unmanaged<F_TaskPromise> Promise;
        TF_SmallVector<F_Name, 1> InitialTags;
        E_TaskStackSize StackSize = E_TaskStackSize::DEFAULT;
    };
    struct ABYTEK_BASE_TASK_API F_TaskInstanceSet : F_Task
    {
        U32 Count = 1;
        U32 BatchSize = 32;
        U32 Offset = 0;
    };

    struct I_TaskScheduler
    {
    protected:
        virtual void OnSchedule(F_TaskInstanceSet&& InstanceSet) = 0;

    public:
        TS_Unmanaged<F_TaskPromise> Schedule(F_TaskInstanceSet&& InstanceSet)
        {
            if (!InstanceSet.Promise)
            {
                InstanceSet.Promise = TS_Unmanaged<F_TaskPromise>()(InstanceSet.Count);
            }
            TS_Unmanaged<F_TaskPromise> CachedPromise = InstanceSet.Promise;
            OnSchedule(ABYTEK_MOVE(InstanceSet));
            return CachedPromise;
        }
        ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(const F_TaskInstanceSet& InstanceSet)
        {
            return Schedule(F_TaskInstanceSet(InstanceSet));
        }
    };
}
