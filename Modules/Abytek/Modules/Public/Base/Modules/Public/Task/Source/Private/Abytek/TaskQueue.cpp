#include "Abytek/TaskQueue.hpp"


namespace Abytek
{
    F_TaskQueue::F_TaskQueue()
    {
    }
    F_TaskQueue::~F_TaskQueue()
    {
        ABYTEK_BASE_TASK_ASSERT(IsEmpty()) << "Task queue was not flushed to be destroyed";
    }
}
