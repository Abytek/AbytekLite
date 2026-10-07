#include "Abytek/TaskScheduler.hpp"
#include "Abytek/TaskWorker.hpp"
#include "Abytek/TaskManager.hpp"


namespace Abytek
{
    namespace Internal::TaskScheduler
    {
        TF_Vector<TW<F_TaskWorker>> GetWorkers(E_TaskWorkerFlag AllRequiredFlags, E_TaskWorkerFlag AnyRequiredFlags)
        {
            TF_Vector<TW<F_TaskWorker>> Result;
            for (const auto& Worker : F_TaskManager::GetInstance()->GetWorkers())
            {
                if (
                    FlagHas(Worker->GetFlags(), AllRequiredFlags)
                    || FlagHasAny(Worker->GetFlags(), AnyRequiredFlags)
                )
                {
                    Result.push_back(Worker);
                }
            }
            return ABYTEK_MOVE(Result);
        }
    }
    
    F_TaskScheduler::F_TaskScheduler(const TF_Span<const TW<F_TaskWorker>>& Workers)
    {
        _Workers = TF_Vector<TW<F_TaskWorker>>(
            Workers.begin(),
            Workers.end()
        );
    }
    F_TaskScheduler::F_TaskScheduler(E_TaskWorkerFlag AllRequiredFlags, E_TaskWorkerFlag AnyRequiredFlags) :
        F_TaskScheduler(Internal::TaskScheduler::GetWorkers(AllRequiredFlags, AnyRequiredFlags))
    {
    }
    F_TaskScheduler::~F_TaskScheduler()
    {
    }

    void F_TaskScheduler::OnSchedule(F_TaskInstanceSet&& InstanceSet)
    {
        auto AcquireWorkerIndex = [this]()
        {
            U32 WorkerIndex = (_SimpleScheduleCounter.fetch_add(1) + 1) % _Workers.size();
            WorkerIndex = Max(WorkerIndex, 1U);
            WorkerIndex %= _Workers.size();
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
