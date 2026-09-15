#include "Abytek/TaskManager.hpp"
#include "Abytek/TaskWorker.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_TaskManager);

    F_TaskManager::F_TaskManager()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_TaskManager::~F_TaskManager()
    {
    }

    void F_TaskManager::PreConfig(const TW_Valid<F_Console>& Console)
    {
        _ConsoleVariable_Profile = Console->RegisterVariable(
            ABYTEK_NAME("Abytek.TaskManager.Profile"),
            ABYTEK_TEXT(""),
            E_TaskManagerProfile::DEFAULT
        );
        _ConsoleVariable_NumWorkers = Console->RegisterVariable<I32>(
            ABYTEK_NAME("Abytek.TaskManager.NumWorkers"),
            ABYTEK_TEXT(""),
            0
        );
        _ConsoleVariable_LowFrequencyWorkerRatio = Console->RegisterVariable<F32>(
            ABYTEK_NAME("Abytek.TaskManager.LowFrequencyWorkerRatio"),
            ABYTEK_TEXT(""),
            0.0f
        );
        _ConsoleVariable_GroupSize = Console->RegisterVariable<I32>(
            ABYTEK_NAME("Abytek.TaskManager.GroupSize"),
            ABYTEK_TEXT(""),
            4
        );
    }
    void F_TaskManager::ApplyConsoleVariables()
    {
        E_TaskManagerProfile Profile = _ConsoleVariable_Profile->GetValue();
        
        _Config.NumWorkers = _ConsoleVariable_NumWorkers->GetValue();
        if (_Config.NumWorkers <= 0)
        {
            auto RatioToTaskNumWorkers = [](F32 Ratio) -> I32
            {
                return Max(
                    RoundToInt(static_cast<F32>(F_Thread::HardwareConcurrency() - 1) * Ratio),
                    1
                );
            };
            switch(Profile)
            {
            case E_TaskManagerProfile::SINGLE_THREADED:
                _Config.NumWorkers = 1;
                break;
            case E_TaskManagerProfile::ULTRA_LOW:
                _Config.NumWorkers = 2;
                break;
            case E_TaskManagerProfile::LOW:
                _Config.NumWorkers = RatioToTaskNumWorkers(0.2f);
                break;
            case E_TaskManagerProfile::MEDIUM:
                _Config.NumWorkers = RatioToTaskNumWorkers(0.33f);
                break;
            case E_TaskManagerProfile::HIGH:
                _Config.NumWorkers = RatioToTaskNumWorkers(0.66f);
                break;
            case E_TaskManagerProfile::ULTRA_HIGH:
                _Config.NumWorkers = RatioToTaskNumWorkers(1.0f);
                break;
            }
        }
        if (IsMemoryLeakDebuggingEnabled())
        {
            _Config.NumWorkers = 1;
        }
        
        _Config.GroupSize = _ConsoleVariable_GroupSize->GetValue();
        _Config.NumGroups = (_Config.NumWorkers + _Config.GroupSize - 1) / _Config.GroupSize;
        
        F32 LowFrequencyWorkerRatio = Min<F32>(
            1.0f,
            Max<F32>(
                _ConsoleVariable_LowFrequencyWorkerRatio->GetValue(),
                0.0f
            )
        );
        _Config.NumLowFrequencyWorkers = Max<U32>(
            1,
            (U32)(LowFrequencyWorkerRatio * static_cast<F32>(_Config.NumWorkers))
        );
        _Config.NumHighFrequencyWorkers = Max<U32>(
            1,
            _Config.NumWorkers - _Config.NumLowFrequencyWorkers
        );
    }
    void F_TaskManager::Start(F_TaskInstanceSet&& MainInstanceSet)
    {
        ApplyConsoleVariables();
        _CreateWorkers();
        _MainPromise = _Workers[0]->Schedule(ABYTEK_MOVE(MainInstanceSet));
        _StartWorkers();
    }
    void F_TaskManager::Join()
    {
        _DestroyWorkers();
    }

    void F_TaskManager::_CreateWorkers()
    {
        for (U32 Idx = 0; Idx < _Config.NumWorkers; ++Idx)
        {
            E_TaskWorkerFlag WorkerFlags = E_TaskWorkerFlag::NONE;
            if (Idx >= (_Config.NumWorkers - _Config.NumLowFrequencyWorkers))
            {
                WorkerFlags |= E_TaskWorkerFlag::LOW_FREQUENCY;
            }
            else
            {
                WorkerFlags |= E_TaskWorkerFlag::HIGH_FREQUENCY;
            }
            if (Idx == 0)
            {
                WorkerFlags |= E_TaskWorkerFlag::MAIN_THREAD;
            }
            else
            {
                WorkerFlags |= E_TaskWorkerFlag::DEDICATED_THREAD;
            }
            
            auto OwnedWorker = TU<F_TaskWorker>()(Idx, WorkerFlags);
            auto Worker = OwnedWorker.Weak();
            _OwnedWorkers.push_back(ABYTEK_MOVE(OwnedWorker));
            _Workers.push_back(Worker);
        }
    }
    void F_TaskManager::_StartWorkers()
    {
        for (I32 Idx = 1; Idx < _Config.NumWorkers; ++Idx)
        {
            _Workers[Idx]->_Start();
        }
        _Workers[0]->_Start();

        while (_Tick())
        {
            _Workers[0]->_Tick();
        }
    }
    void F_TaskManager::_DestroyWorkers()
    {
        // Wait for all task queues to be empty
        {
            B8 ShouldContinueWaiting = true;
            while (ShouldContinueWaiting)
            {
                ShouldContinueWaiting = false;
                _Workers[0]->_Tick();
                for (I32 Idx = 0; Idx < _Config.NumWorkers; ++Idx)
                {
                    if (!_Workers[Idx]->GetQueue().IsEmpty())
                    {
                        ShouldContinueWaiting = true;
                    }
                }
            }
        }

        // Join, destroy workers
        for (I32 Idx = 0; Idx < _Config.NumWorkers; ++Idx)
        {
            _Workers[Idx]->_Join();
        }
        _OwnedWorkers.clear();
    }

    B8 F_TaskManager::_Tick()
    {
        return !(_MainPromise->IsDone());
    }

    void F_TaskManager::OnSchedule(F_TaskInstanceSet&& InstanceSet)
    {
        auto AcquireWorkerIndex = [this]()
        {
            U32 WorkerIndex = (_SimpleScheduleCounter.fetch_add(1) + 1) % _Config.NumWorkers;
            WorkerIndex = Max(WorkerIndex, 1U);
            WorkerIndex %= _Config.NumWorkers;
            return WorkerIndex;
        };
        
        if (InstanceSet.Count == 1)
        {
            _Workers[AcquireWorkerIndex()]->Schedule(ABYTEK_MOVE(InstanceSet));
        }
        else
        {
            U32 OriginalCount = InstanceSet.Count;
            U32 OriginalBatchSize = InstanceSet.BatchSize;
            U32 NumBatches = (OriginalCount + OriginalBatchSize - 1) / OriginalBatchSize;
            
            for (U32 Idx = 1; Idx < NumBatches; ++Idx)
            {
                F_TaskInstanceSet CloneInstanceSet = InstanceSet;
                CloneInstanceSet.Count = Min(CloneInstanceSet.Count, OriginalBatchSize);
                
                InstanceSet.Offset += CloneInstanceSet.Count;
                InstanceSet.Count -= CloneInstanceSet.Count;

                _Workers[AcquireWorkerIndex()]->Schedule(ABYTEK_MOVE(CloneInstanceSet));
            }
            
            _Workers[AcquireWorkerIndex()]->Schedule(ABYTEK_MOVE(InstanceSet));
        }
    }
}
