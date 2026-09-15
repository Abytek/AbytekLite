#include "Abytek/TaskCondition.hpp"


namespace Abytek
{
    A_TaskCondition::A_TaskCondition()
    {
    }
    A_TaskCondition::~A_TaskCondition()
    {
    }

    F_TaskWaitForSeconds::F_TaskWaitForSeconds(float TargetSeconds) :
        _StartTime(boost::chrono::high_resolution_clock::now()),
        _TargetSeconds(TargetSeconds)
    {
    }
    F_TaskWaitForSeconds::~F_TaskWaitForSeconds()
    {
    }

    B8 F_TaskWaitForSeconds::ShouldContinue()
    {
        auto Elapsed = boost::chrono::duration_cast<boost::chrono::duration<float>>(
            boost::chrono::high_resolution_clock::now() - _StartTime
        ).count();
        return Elapsed >= _TargetSeconds;
    }

    F_TaskWaitForPromise::F_TaskWaitForPromise(const TS_ValidUnmanaged<F_TaskPromise>& Promise) :
        _Promise(Promise)
    {
    }
    F_TaskWaitForPromise::~F_TaskWaitForPromise()
    {
    }

    B8 F_TaskWaitForPromise::ShouldContinue()
    {
        return _Promise->IsDone();
    }
}
