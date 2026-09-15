#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"
#include "Abytek/Task.hpp"
#include "Abytek/TaskPromise.hpp"
#include "Abytek/TaskFuture.hpp"
#include "Abytek/TaskContext.hpp"
#include "Abytek/TaskWorker.hpp"
#include "Abytek/TaskManager.hpp"


namespace Abytek
{
    class F_TaskPromise;
    class F_TaskWorker;
    struct F_TaskSubsystemConfig;
    
    struct ABYTEK_BASE_TASK_API H_TaskUtilities
    {
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(const TW_Valid<I_TaskScheduler>& Scheduler, F_TaskInstanceSet&& InstanceSet)
        {
            return Scheduler->Schedule(ABYTEK_MOVE(InstanceSet));
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(const TW_Valid<I_TaskScheduler>& Scheduler, const F_TaskInstanceSet& InstanceSet)
        {
            return Schedule(Scheduler, F_TaskInstanceSet(InstanceSet));
        }
        
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(const TW_Valid<I_TaskScheduler>& Scheduler, F_TaskFunctor&& Functor, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                Scheduler,
                ABYTEK_MOVE(InstanceSet)
            );
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(const TW_Valid<I_TaskScheduler>& Scheduler, F_TaskFunctor&& Functor, E_TaskPriority Priority, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Priority = Priority;
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                Scheduler,
                ABYTEK_MOVE(InstanceSet)
            );
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(const TW_Valid<I_TaskScheduler>& Scheduler, F_TaskFunctor&& Functor, E_TaskPriority Priority, const F_Name& Name, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Priority = Priority;
            InstanceSet.Name = Name;
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                Scheduler,
                ABYTEK_MOVE(InstanceSet)
            );
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(const TW_Valid<I_TaskScheduler>& Scheduler, F_TaskFunctor&& Functor, const TS_Unmanaged<F_TaskPromise>& Promise, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Promise = Promise;
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                Scheduler,
                ABYTEK_MOVE(InstanceSet)
            );
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(const TW_Valid<I_TaskScheduler>& Scheduler, F_TaskFunctor&& Functor, E_TaskPriority Priority, const TS_Unmanaged<F_TaskPromise>& Promise, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Priority = Priority;
            InstanceSet.Promise = Promise;
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                Scheduler,
                ABYTEK_MOVE(InstanceSet)
            );
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(const TW_Valid<I_TaskScheduler>& Scheduler, F_TaskFunctor&& Functor, E_TaskPriority Priority, const F_Name& Name, const TS_Unmanaged<F_TaskPromise>& Promise, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Priority = Priority;
            InstanceSet.Name = Name;
            InstanceSet.Promise = Promise;
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                Scheduler,
                ABYTEK_MOVE(InstanceSet)
            );
        }
        
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(F_TaskInstanceSet&& InstanceSet)
        {
            return Schedule(F_TaskManager::GetInstance(), ABYTEK_FORWARD(InstanceSet));
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(const F_TaskInstanceSet& InstanceSet)
        {
            return Schedule(F_TaskManager::GetInstance(), ABYTEK_FORWARD(InstanceSet));
        }
        
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(F_TaskFunctor&& Functor, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                F_TaskManager::GetInstance(),
                ABYTEK_MOVE(InstanceSet)
            );
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(F_TaskFunctor&& Functor, E_TaskPriority Priority, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Priority = Priority;
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                F_TaskManager::GetInstance(),
                ABYTEK_MOVE(InstanceSet)
            );
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(F_TaskFunctor&& Functor, E_TaskPriority Priority, const F_Name& Name, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Priority = Priority;
            InstanceSet.Name = Name;
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                F_TaskManager::GetInstance(),
                ABYTEK_MOVE(InstanceSet)
            );
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(F_TaskFunctor&& Functor, const TS_Unmanaged<F_TaskPromise>& Promise, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Promise = Promise;
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                F_TaskManager::GetInstance(),
                ABYTEK_MOVE(InstanceSet)
            );
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(F_TaskFunctor&& Functor, E_TaskPriority Priority, const TS_Unmanaged<F_TaskPromise>& Promise, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Priority = Priority;
            InstanceSet.Promise = Promise;
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                F_TaskManager::GetInstance(),
                ABYTEK_MOVE(InstanceSet)
            );
        }
        static ABYTEK_FORCE_INLINE TS_Unmanaged<F_TaskPromise> Schedule(F_TaskFunctor&& Functor, E_TaskPriority Priority, const F_Name& Name, const TS_Unmanaged<F_TaskPromise>& Promise, U32 Count = 1, U32 BatchSize = 32, U32 Offset = 0)
        {
            F_TaskInstanceSet InstanceSet;
            InstanceSet.Functor = ABYTEK_MOVE(Functor);
            InstanceSet.Priority = Priority;
            InstanceSet.Name = Name;
            InstanceSet.Promise = Promise;
            InstanceSet.Count = Count;
            InstanceSet.BatchSize = BatchSize;
            InstanceSet.Offset = Offset;
            return Schedule(
                F_TaskManager::GetInstance(),
                ABYTEK_MOVE(InstanceSet)
            );
        }

        ABYTEK_FORCE_INLINE static const auto& GetInstanceSet()
        {
            return F_TaskContext::GetCurrentRef().GetInstanceSet();
        }
        ABYTEK_FORCE_INLINE static E_TaskPriority GetPriority()
        {
            return GetInstanceSet().Priority;
        }
        ABYTEK_FORCE_INLINE static void ChangePriority(E_TaskPriority Priority)
        {
            F_TaskContext::GetCurrentRef().ChangePriority(Priority);
        }
        ABYTEK_FORCE_INLINE static const TS_Unmanaged<F_TaskPromise>& GetPromise()
        {
            return F_TaskContext::GetCurrentRef().GetInstanceSet().Promise;
        }
        template<typename __F_Data>
        ABYTEK_FORCE_INLINE static const TS_Unmanaged<TF_TaskFuture<__F_Data>>& GetFuture()
        {
            return GetPromise().FastCast<TF_TaskFuture<__F_Data>>();
        }
        template<typename __F_Data>
        ABYTEK_FORCE_INLINE static __F_Data& GetFutureData()
        {
            return GetFuture<__F_Data>()->Data;
        }
        ABYTEK_FORCE_INLINE static const auto& GetInitialTags()
        {
            return GetInstanceSet().InitialTags;
        }
        ABYTEK_FORCE_INLINE static void Switch()
        {
            F_TaskContext::GetCurrentRef().Switch();
        }
        ABYTEK_FORCE_INLINE static void Wait(A_TaskCondition&& Condition)
        {
            F_TaskContext::GetCurrentRef().Wait(ABYTEK_FORWARD(Condition));
        }
        ABYTEK_FORCE_INLINE static void Wait(const TS_ValidUnmanaged<F_TaskPromise>& Promise)
        {
            F_TaskContext::GetCurrentRef().Wait(F_TaskWaitForPromise(Promise));
        }

        ABYTEK_FORCE_INLINE static U32 GetCurrentInstanceIndex()
        {
            return F_TaskContext::GetCurrentInstanceIndex();
        }
        ABYTEK_FORCE_INLINE static const auto& GetTags()
        {
            return F_TaskContext::GetCurrentRef().GetInstanceIndexToTags()[F_TaskContext::GetCurrentInstanceIndex()];
        }
        ABYTEK_FORCE_INLINE static auto& AccessTags()
        {
            return F_TaskContext::GetCurrentRef().GetInstanceIndexToTags()[F_TaskContext::GetCurrentInstanceIndex()];
        }
        ABYTEK_FORCE_INLINE static void AddTag(const F_Name& Tag)
        {
            auto& Tags = AccessTags();
            Tags.push_back(Tag);
        }
        ABYTEK_FORCE_INLINE static void RemoveTag(const F_Name& Tag)
        {
            auto& Tags = AccessTags();
            Tags.erase(
                std::find(
                    Tags.begin(),
                    Tags.end(),
                    Tag
                )  
            );
        }
        ABYTEK_FORCE_INLINE static B8 HasTag(const F_Name& InTag)
        {
            for (const auto& Tag : GetTags())
            {
                if (Tag == InTag)
                {
                    return true;
                }
            }
            return false;
        }
        ABYTEK_FORCE_INLINE static auto GetId()
        {
            const auto& TaskContext = F_TaskContext::GetCurrentRef();
            return TaskContext.GetBeginTaskId() + TaskContext.GetCurrentInstanceIndex();
        }
        
        static const F_TaskSubsystemConfig& GetConfig();
        
        static TW_Valid<F_TaskWorker> GetWorker(U32 Index);
        static TW_Valid<F_TaskWorker> GetWorker(E_TaskWorkerFlag Flags, U32 SearchedIndex = 0);
        static U32 GetNumWorkers();
    };

    struct F_TaskPromiseAwait
    {
        friend TS_Unmanaged<F_TaskPromise> operator << (const F_TaskPromiseAwait&, const TS_Unmanaged<F_TaskPromise>& Promise)
        {
            if (!Promise)
            {
                return {};
            }
            H_TaskUtilities::Wait(Promise);
            return Promise;
        }
        friend void operator << (const F_TaskPromiseAwait&, const TF_Vector<TS_ValidUnmanaged<F_TaskPromise>>& Promises)
        {
            for (const auto& Promise : Promises)
            {
                H_TaskUtilities::Wait(Promise);
            }
        }
        friend void operator << (const F_TaskPromiseAwait&, const TF_Span<TS_ValidUnmanaged<F_TaskPromise>>& Promises)
        {
            for (const auto& Promise : Promises)
            {
                H_TaskUtilities::Wait(Promise);
            }
        }
        friend void operator << (const F_TaskPromiseAwait&, const TF_Vector<TS_Unmanaged<F_TaskPromise>>& Promises)
        {
            for (const auto& Promise : Promises)
            {
                if (Promise)
                {
                    H_TaskUtilities::Wait(Promise);
                }
            }
        }
        friend void operator << (const F_TaskPromiseAwait&, const TF_Span<TS_Unmanaged<F_TaskPromise>>& Promises)
        {
            for (const auto& Promise : Promises)
            {
                if (Promise)
                {
                    H_TaskUtilities::Wait(Promise);
                }
            }
        }
        
        template<typename __F_Data>
        friend TS_Unmanaged<TF_TaskFuture<__F_Data>> operator << (const F_TaskPromiseAwait&, const TS_Unmanaged<TF_TaskFuture<__F_Data>>& Promise)
        {
            if (!Promise)
            {
                return {};
            }
            H_TaskUtilities::Wait(Promise);
            return Promise;
        }
        template<typename __F_Data>
        friend void operator << (const F_TaskPromiseAwait&, const TF_Vector<TS_ValidUnmanaged<TF_TaskFuture<__F_Data>>>& Promises)
        {
            for (const auto& Promise : Promises)
            {
                H_TaskUtilities::Wait(Promise);
            }
        }
        template<typename __F_Data>
        friend void operator << (const F_TaskPromiseAwait&, const TF_Span<TS_ValidUnmanaged<TF_TaskFuture<__F_Data>>>& Promises)
        {
            for (const auto& Promise : Promises)
            {
                H_TaskUtilities::Wait(Promise);
            }
        }
        template<typename __F_Data>
        friend void operator << (const F_TaskPromiseAwait&, const TF_Vector<TS_Unmanaged<TF_TaskFuture<__F_Data>>>& Promises)
        {
            for (const auto& Promise : Promises)
            {
                if (Promise)
                {
                    H_TaskUtilities::Wait(Promise);
                }
            }
        }
        template<typename __F_Data>
        friend void operator << (const F_TaskPromiseAwait&, const TF_Span<TS_Unmanaged<TF_TaskFuture<__F_Data>>>& Promises)
        {
            for (const auto& Promise : Promises)
            {
                if (Promise)
                {
                    H_TaskUtilities::Wait(Promise);
                }
            }
        }
    };
}

#define ABYTEK_AWAIT Abytek::F_TaskPromiseAwait {} << 
#define ABYTEK_CHECK_TASK_TAG(...) ABYTEK_ASSERT_GENERAL(Abytek::H_TaskUtilities::HasTag(__VA_ARGS__)) << "Requires task tag: " << __VA_ARGS__;
#define ABYTEK_CHECK_NOT_TASK_TAG(...) ABYTEK_ASSERT_GENERAL(!Abytek::H_TaskUtilities::HasTag(__VA_ARGS__)) << "Task tag not allowed: " << __VA_ARGS__;