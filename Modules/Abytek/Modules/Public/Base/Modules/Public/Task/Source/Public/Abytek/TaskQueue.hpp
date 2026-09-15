#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"
#include "Abytek/Task.hpp"
#include "Abytek/TaskContext.hpp"


namespace Abytek
{
    class F_TaskContext;
    
    struct ABYTEK_BASE_TASK_API F_TaskQueue
    {
    public:
        TF_Array<
            TF_ConcurrentQueue<F_TaskInstanceSet>,
            static_cast<U32>(E_TaskPriority::NUM)
        > InstanceSetQueues;
        TF_Array<
            TF_Queue<TS_Unmanaged<F_TaskContext>>,
            static_cast<U32>(E_TaskPriority::NUM)
        > PausedContextQueues;
        AU32 Counter = 0;
        
        TF_Vector<F_TaskInstanceSet> _TempInstanceSets;
        TF_Vector<TS_Unmanaged<F_TaskContext>> _TempPausedContexts;

    public:
        ABYTEK_FORCE_INLINE B8 IsEmpty() const noexcept
        {
            return Counter.load(boost::memory_order_acquire) == 0;
        }

    public:
        F_TaskQueue();
        ~F_TaskQueue();

    public:
        template<typename __F_PassedCallback>
        void TryPopInstanceSets(__F_PassedCallback&& Callback)
        {
            for (I32 PriorityIndex = static_cast<I32>(E_TaskPriority::NUM) - 1; PriorityIndex >= 0; --PriorityIndex)
            {
                auto& InstanceSetQueue = InstanceSetQueues[PriorityIndex];
                
                E_TaskPriority Priority = static_cast<E_TaskPriority>(PriorityIndex);
                U32 NumInstanceSetsPerCycle = ConvertTaskPriorityToNumInstanceSetsPerCycle(Priority);
                for (U32 InstanceSetIndex = 0; InstanceSetIndex < NumInstanceSetsPerCycle; ++InstanceSetIndex)
                {
                    F_TaskInstanceSet InstanceSet;
                    if (InstanceSetQueue.TryPop(InstanceSet))
                    {
                        _TempInstanceSets.push_back(ABYTEK_FORWARD(InstanceSet));
                        Counter.fetch_sub(1, boost::memory_order_release);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            for (auto& InstanceSet : _TempInstanceSets)
            {
                Callback(ABYTEK_MOVE(InstanceSet));
            }
            _TempInstanceSets.clear();
        }
        template<typename __F_PassedCallback>
        void TryPopPausedContexts(__F_PassedCallback&& Callback)
        {
            for (I32 PriorityIndex = static_cast<I32>(E_TaskPriority::NUM) - 1; PriorityIndex >= 0; --PriorityIndex)
            {
                auto& PausedContextQueue = PausedContextQueues[PriorityIndex];
                
                E_TaskPriority Priority = static_cast<E_TaskPriority>(PriorityIndex);
                U32 NumInstanceSetsPerCycle = ConvertTaskPriorityToNumInstanceSetsPerCycle(Priority);
                for (U32 InstanceSetIndex = 0; InstanceSetIndex < NumInstanceSetsPerCycle; ++InstanceSetIndex)
                {
                    TS_Unmanaged<F_TaskContext> PausedContext;
                    if (PausedContextQueue.TryPop(PausedContext))
                    {
                        _TempPausedContexts.push_back(ABYTEK_FORWARD(PausedContext));
                        Counter.fetch_sub(1, boost::memory_order_release);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            for (auto& PausedContext : _TempPausedContexts)
            {
                Callback(ABYTEK_MOVE(PausedContext));
            }
            _TempPausedContexts.clear();
        }

    public:
        void Push(F_TaskInstanceSet&& InstanceSet)
        {
            U32 PriorityIndex = static_cast<U32>(InstanceSet.Priority);
            Counter.fetch_add(1, boost::memory_order_release);
            InstanceSetQueues[PriorityIndex].Push(ABYTEK_MOVE(InstanceSet));
        }
        void Push(const F_TaskInstanceSet& InstanceSet)
        {
            Push(F_TaskInstanceSet(InstanceSet));
        }
        void Push(TS_Unmanaged<F_TaskContext>&& PausedContext)
        {
            U32 PriorityIndex = static_cast<U32>(PausedContext->GetInstanceSet().Priority);
            Counter.fetch_add(1, boost::memory_order_release);
            PausedContextQueues[PriorityIndex].Push(ABYTEK_MOVE(PausedContext));
        }
        void Push(const TS_Unmanaged<F_TaskContext>& PausedContext)
        {
            Push(TS_Unmanaged<F_TaskContext>(PausedContext));
        }
    };
}
