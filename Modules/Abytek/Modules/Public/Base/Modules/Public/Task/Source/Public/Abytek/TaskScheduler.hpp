#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"
#include "Abytek/Task.hpp"
#include "Abytek/TaskWorkerFlag.hpp"


namespace Abytek
{
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
    
    class ABYTEK_BASE_TASK_API F_TaskScheduler : public A_Object, public I_TaskScheduler
    {
    private:
        TF_Vector<TW<F_TaskWorker>> _Workers;
        AU32 _SimpleScheduleCounter = 0;

    public:
        ABYTEK_FORCE_INLINE const auto& GetWorkers() const noexcept
        {
            return _Workers;
        }
        
    public:
        F_TaskScheduler(const TF_Span<const TW<F_TaskWorker>>& Workers);
        F_TaskScheduler(E_TaskWorkerFlag AllRequiredFlags, E_TaskWorkerFlag AnyRequiredFlags = E_TaskWorkerFlag::NONE);
        ~F_TaskScheduler() override;

    protected:
        void OnSchedule(F_TaskInstanceSet&& InstanceSet) override;
    };
}
