#include "Abytek/Frame/FrameParamTask.hpp"


namespace Abytek
{
    /*
    void F_FrameParamQueue::BeginFrame()
    {
        auto& TaskList = GetCurrentTaskList();
        BeginEvent = TS_Unmanaged<F_TaskEvent>()();
        TS_Unmanaged<F_TaskEvent> LastEvent = BeginEvent;
        F_FrameParamTask Task;
        while (TaskList.TryPop(Task))
        {
            Task.BeginEvent->AddDependency(LastEvent.Weak());
            if (Task.EndEvent != Task.BeginEvent)
            {
                Task.EndEvent->AddDependency(Task.BeginEvent.Weak());
            }
            if (Task.RunnableIndex == -1)
            {
                Task.EndEvent->Future(Task.Desc);
            }
            else
            {
                Task.EndEvent->Future(
                    F_TaskSubsystem::GetInstance()->GetRunnables()[Task.RunnableIndex],
                    Task.Desc
                );
            }
            LastEvent = Task.EndEvent;
        }
        EndEvent = TS_Unmanaged<F_TaskEvent>()();
        EndEvent->AddDependency(LastEvent.Weak());

        BeginEvent->Broadcast();
    }
    void F_FrameParamQueue::EndFrame()
    {
        EndEvent->Wait();
        
        constexpr auto NumTaskLists = static_cast<F_FrameParamIndex>(TaskLists.size());
        auto CachedTaskLists = ABYTEK_MOVE(TaskLists);
        for (F_FrameParamIndex Idx = 0; Idx < NumTaskLists; ++Idx)
        {
            F_FrameParamIndex TargetIdx = (
                (Idx + NumTaskLists - 1)
                % NumTaskLists
            );
            TaskLists[Idx] = ABYTEK_MOVE(CachedTaskLists[TargetIdx]);
        }
    }

    void F_FrameParamQueue::Schedule(
        const F_TaskDesc& Desc,
        U32 RunnableIndex,
        const TS_Unmanaged<F_TaskEvent>& BeginEvent,
        const TS_Unmanaged<F_TaskEvent>& EndEvent,
        F_FrameParamIndex Delay
    )
    {
        F_FrameParamTask Task;
        Task.Desc = Desc;
        Task.BeginEvent = BeginEvent;
        Task.EndEvent = EndEvent;
        GetCurrentTaskList(Delay).Push(
            ABYTEK_MOVE(Task)
        );
    }
    void F_FrameParamQueue::Schedule(
        const F_TaskDesc& Desc,
        U32 RunnableIndex,
        const TS_Unmanaged<F_TaskEvent>& Event,
        F_FrameParamIndex Delay
    )
    {
        Schedule(
            Desc,
            RunnableIndex,
            Event,
            Event,
            Delay
        );
    }

    ABYTEK_REFLECT(F_FrameParamTaskSubsystem)
    {
    }

    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_FrameParamTaskSubsystem);

    F_FrameParamTaskSubsystem::F_FrameParamTaskSubsystem()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();

        for (F_FrameParamIndex Idx = 0; Idx < _Queues.size(); ++Idx)
        {
            _Queues[Idx].Param = static_cast<E_FrameParam>(Idx + 1);
        }
    }
    F_FrameParamTaskSubsystem::~F_FrameParamTaskSubsystem()
    {
    }

    B8 F_FrameParamTaskSubsystem::OnPostInit()
    {
        return A_ApplicationSubsystem::OnPostInit();
    }
    B8 F_FrameParamTaskSubsystem::OnPreRelease()
    {
        return A_ApplicationSubsystem::OnPreRelease();
    }
    B8 F_FrameParamTaskSubsystem::Release()
    {
        return A_ApplicationSubsystem::Release();
    }

    void F_FrameParamTaskSubsystem::BeginFrame()
    {
        for (auto& Queue: _Queues)
        {
            Queue.BeginFrame();
        }
    }
    void F_FrameParamTaskSubsystem::EndFrame()
    {
        for (auto& Queue: _Queues)
        {
            Queue.EndFrame();
        }
    }
    void F_FrameParamTaskSubsystem::Schedule(
        E_FrameParam FrameParam,
        const F_TaskDesc& Desc,
        U32 RunnableIndex,
        const TS_Unmanaged<F_TaskEvent>& BeginEvent,
        const TS_Unmanaged<F_TaskEvent>& EndEvent,
        F_FrameParamIndex Delay
    )
    {
        ABYTEK_ENGINE_CORE_ASSERT(FrameParam != E_FrameParam::MAIN);
        _Queues[static_cast<F_FrameParamIndex>(FrameParam) - 1].Schedule(
            Desc,
            RunnableIndex,
            BeginEvent,
            EndEvent,
            Delay
        );
    }
    void F_FrameParamTaskSubsystem::Schedule(
        E_FrameParam FrameParam,
        const F_TaskDesc& Desc,
        U32 RunnableIndex,
        const TS_Unmanaged<F_TaskEvent>& Event,
        F_FrameParamIndex Delay
    )
    {
        Schedule(
            FrameParam,
            Desc,
            RunnableIndex,
            Event,
            Event,
            Delay
        );
    }
    */
}
