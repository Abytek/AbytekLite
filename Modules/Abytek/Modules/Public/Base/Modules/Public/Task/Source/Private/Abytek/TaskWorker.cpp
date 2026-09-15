#include "Abytek/TaskWorker.hpp"
#include "Abytek/Profiling.hpp"
#include "Abytek/TaskContext.hpp"
#include "Abytek/TaskContextPool.hpp"
#include "Abytek/TaskManager.hpp"


namespace Abytek
{
    namespace Internal
    {
        thread_local F_TaskWorker* CurrentTaskWorkerRaw;    
        thread_local F_StandardObjectKey CurrentTaskWorkerObjectKey;    
    }

    F_TaskWorker::F_TaskWorker(U32 Index, E_TaskWorkerFlag Flags) :
        _Index(Index), 
        _Flags(Flags)
    {
        for (F_TaskStackSizeIndex StackSizeIndex = 0; StackSizeIndex < static_cast<F_TaskStackSizeIndex>(E_TaskStackSize::NUM); ++StackSizeIndex)
        {
            _ContextPools[StackSizeIndex] = TS<F_TaskContextPool>()(
                ResolveTaskStackSize(
                    static_cast<E_TaskStackSize>(StackSizeIndex)
                )
            );    
        }
        
        _Name = F_Text(ABYTEK_TEXT("Abytek.TaskWorker("))
            + ABYTEK_TEXT("Index = ") + ToText(Index)
            + (FlagHas(Flags, E_TaskWorkerFlag::LOW_FREQUENCY) ? ABYTEK_TEXT(", Low Frequency") : ABYTEK_TEXT(""))
            + (FlagHas(Flags, E_TaskWorkerFlag::HIGH_FREQUENCY) ? ABYTEK_TEXT(", High Frequency") : ABYTEK_TEXT(""))
            + (FlagHas(Flags, E_TaskWorkerFlag::MAIN_THREAD) ? ABYTEK_TEXT(", Main") : ABYTEK_TEXT(""))
            + ABYTEK_TEXT(")");
        _NameString = ToString(*_Name);
        
        _ShouldContinue.store(true, boost::memory_order_release);

        _MaxIdleTicksToSleep = 8;
        _IdleTickCounter = 0;
    }
    F_TaskWorker::~F_TaskWorker()
    {
    }

    void F_TaskWorker::_Start()
    {
        auto TaskManager = F_TaskManager::GetInstance();
        
        if (FlagHas(_Flags, E_TaskWorkerFlag::DEDICATED_THREAD))
        {
            _Thread = F_Thread(
                [this]()
                {
                    ABYTEK_PROFILER_THREAD_NAMED("Abytek::TaskWorker");
                    
                    _Bind();
                    while (_ShouldContinue.load(boost::memory_order_acquire))
                    {
                        _Tick();
                    }
                    _Unbind();
                },
                _Name
            );
        }

        if (FlagHas(_Flags, E_TaskWorkerFlag::MAIN_THREAD))
        {
            _Bind();
        }
    }
    void F_TaskWorker::_Join()
    {
        if (FlagHas(_Flags, E_TaskWorkerFlag::MAIN_THREAD))
        {
            _Unbind();
        }
        
        _ShouldContinue.store(false, boost::memory_order_release);
        
        if (FlagHas(_Flags, E_TaskWorkerFlag::DEDICATED_THREAD))
        {
            _Thread.Join();
        }
    }

    void F_TaskWorker::_Bind()
    {
        Internal::CurrentTaskWorkerRaw = this;
        Internal::CurrentTaskWorkerObjectKey = GetObjectKey<F_DefaultObjectManagement>(this);

#ifdef ABYTEK_ENABLE_PROFILER
        ABYTEK_PROFILER_FIBER_ENTER(_NameString.c_str());
#endif
    }
    void F_TaskWorker::_Unbind()
    {
#ifdef ABYTEK_ENABLE_PROFILER
        ABYTEK_PROFILER_FIBER_LEAVE();
#endif
        
        Internal::CurrentTaskWorkerRaw = nullptr;
        Internal::CurrentTaskWorkerObjectKey = {};
    }
    void F_TaskWorker::_Tick()
    {
        // ABYTEK_PROFILER_EVENT();
        
        if (_ProcessTasks())
        {
            _IdleTickCounter = 0;
        }
        else
        {
            ++_IdleTickCounter;
            if (_IdleTickCounter >= _MaxIdleTicksToSleep)
            {
                H_ThisThread::YieldThread();
            }
        }
    }
    B8 F_TaskWorker::_ProcessTasks()
    {
        // ABYTEK_PROFILER_EVENT();

        B8 HasWorks = false;
        
        _Queue.TryPopInstanceSets(
            [this, &HasWorks](F_TaskInstanceSet&& InstanceSet)
            {
                HasWorks = true;
                
                auto Context = _ContextPools[static_cast<F_TaskStackSizeIndex>(InstanceSet.StackSize)]->Pop();
                Context->_SetupTask(ABYTEK_MOVE(InstanceSet));
                if (
                    Context->_Resume(
                        [this](F_TaskInstanceSet&& SplitedInstanceSet)
                        {
                            _Queue.Push(ABYTEK_MOVE(SplitedInstanceSet));
                        }
                    ) 
                )
                {
                    _Queue.Push(ABYTEK_MOVE(Context));
                }
                else
                {
                    Context->GetPool()->Push(ABYTEK_MOVE(Context));
                }
            }
        );
        _Queue.TryPopPausedContexts(
            [this, &HasWorks](TS_Unmanaged<F_TaskContext>&& PausedContext)
            {
                HasWorks = true;
                
                if (
                    PausedContext->_Resume(
                        [this](F_TaskInstanceSet&& SplitedInstanceSet)
                        {
                            _Queue.Push(ABYTEK_MOVE(SplitedInstanceSet));
                        }
                    )
                )
                {
                    _Queue.Push(ABYTEK_MOVE(PausedContext));
                }
                else
                {
                    PausedContext->GetPool()->Push(ABYTEK_MOVE(PausedContext));
                }
            }
        );
        return HasWorks;
    }

    TW<F_TaskWorker> F_TaskWorker::GetCurrent()
    {
        TW<F_TaskWorker> Result(Internal::CurrentTaskWorkerRaw, Internal::CurrentTaskWorkerObjectKey);
        if (Result)
        {
            return Result;
        }
        return {};
    }

    TW<F_TaskWorker> F_TaskWorker::Search(E_TaskWorkerFlag Flags, U32 SearchedIndex)
    {
        U32 NextSearchedIndex = 0;
        auto TaskManager = F_TaskManager::GetInstance();
        const auto& TaskWorkers = TaskManager->GetWorkers();
        for (U32 Idx = 0; Idx < TaskWorkers.size(); ++Idx)
        {
            const auto& TaskWorker = TaskWorkers[Idx];
            if (FlagHas(TaskWorker->GetFlags(), Flags))
            {
                if (SearchedIndex == NextSearchedIndex)
                {
                    return TaskWorker;
                }
                ++NextSearchedIndex;
            }
        }
        return {};
    }

    void F_TaskWorker::OnSchedule(F_TaskInstanceSet&& InstanceSet)
    {
        ABYTEK_BASE_TASK_ASSERT(_ShouldContinue.load(boost::memory_order_acquire)) << "Cannot schedule task instance set when the task worker is stopped";
        _Queue.Push(InstanceSet);
    }
}
