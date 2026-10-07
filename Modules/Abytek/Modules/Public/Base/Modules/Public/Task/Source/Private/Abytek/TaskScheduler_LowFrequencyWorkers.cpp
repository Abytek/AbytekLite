#include "Abytek/TaskScheduler_LowFrequencyWorkers.hpp"
#include "Abytek/TaskManager.hpp"
#include "Abytek/TaskWorker.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_TaskScheduler_LowFrequencyWorkers);
    
    F_TaskScheduler_LowFrequencyWorkers::F_TaskScheduler_LowFrequencyWorkers() :
        F_TaskScheduler(E_TaskWorkerFlag::LOW_FREQUENCY)
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_TaskScheduler_LowFrequencyWorkers::~F_TaskScheduler_LowFrequencyWorkers()
    {
    }
}
