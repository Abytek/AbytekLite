#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"
#include "Abytek/TaskScheduler.hpp"


namespace Abytek
{
    class F_TaskWorker;
    
    class ABYTEK_BASE_TASK_API F_TaskScheduler_HighFrequencyWorkers final : public F_TaskScheduler
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_TaskScheduler_HighFrequencyWorkers);

    public:
        F_TaskScheduler_HighFrequencyWorkers();
        ~F_TaskScheduler_HighFrequencyWorkers() override;
    };
}
