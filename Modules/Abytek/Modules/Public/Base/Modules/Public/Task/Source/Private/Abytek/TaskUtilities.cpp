#include "Abytek/TaskUtilities.hpp"
#include "Abytek/TaskManager.hpp"


namespace Abytek
{
    const F_TaskSubsystemConfig& H_TaskUtilities::GetConfig()
    {
        return F_TaskManager::GetInstance()->GetConfig();
    }

    TW_Valid<F_TaskWorker> H_TaskUtilities::GetWorker(U32 Index)
    {
        return F_TaskManager::GetInstance()->GetWorkers()[Index];
    }
    TW_Valid<F_TaskWorker> H_TaskUtilities::GetWorker(E_TaskWorkerFlag Flags, U32 SearchedIndex)
    {
        return F_TaskWorker::Search(Flags, SearchedIndex);
    }

    U32 H_TaskUtilities::GetNumWorkers()
    {
        return GetConfig().NumWorkers;
    }
}
