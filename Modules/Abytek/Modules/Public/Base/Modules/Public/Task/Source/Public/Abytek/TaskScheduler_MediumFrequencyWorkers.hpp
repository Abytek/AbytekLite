#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"
#include "Abytek/TaskScheduler.hpp"


namespace Abytek
{
    class F_TaskWorker;
    
    class ABYTEK_BASE_TASK_API F_TaskScheduler_MediumFrequencyWorkers final : public F_TaskScheduler
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_TaskScheduler_MediumFrequencyWorkers);

    public:
        F_TaskScheduler_MediumFrequencyWorkers();
        ~F_TaskScheduler_MediumFrequencyWorkers() override;
    };
}
