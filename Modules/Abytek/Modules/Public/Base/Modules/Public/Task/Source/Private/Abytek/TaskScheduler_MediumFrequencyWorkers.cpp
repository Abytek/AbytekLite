#include "Abytek/TaskScheduler_MediumFrequencyWorkers.hpp"
#include "Abytek/TaskManager.hpp"
#include "Abytek/TaskWorker.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_TaskScheduler_MediumFrequencyWorkers);
    
    F_TaskScheduler_MediumFrequencyWorkers::F_TaskScheduler_MediumFrequencyWorkers() :
        F_TaskScheduler(E_TaskWorkerFlag::MEDIUM_FREQUENCY)
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_TaskScheduler_MediumFrequencyWorkers::~F_TaskScheduler_MediumFrequencyWorkers()
    {
    }
}
