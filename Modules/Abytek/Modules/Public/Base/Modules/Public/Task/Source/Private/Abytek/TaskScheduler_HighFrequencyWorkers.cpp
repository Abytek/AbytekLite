#include "Abytek/TaskScheduler_HighFrequencyWorkers.hpp"
#include "Abytek/TaskManager.hpp"
#include "Abytek/TaskWorker.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_TaskScheduler_HighFrequencyWorkers);
    
    F_TaskScheduler_HighFrequencyWorkers::F_TaskScheduler_HighFrequencyWorkers() :
        F_TaskScheduler(E_TaskWorkerFlag::HIGH_FREQUENCY)
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_TaskScheduler_HighFrequencyWorkers::~F_TaskScheduler_HighFrequencyWorkers()
    {
    }
}
