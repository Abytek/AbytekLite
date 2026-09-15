#include "Abytek/TaskPromise.hpp"


namespace Abytek
{
    F_TaskPromise::F_TaskPromise(U32 InitialCounter) :
        _Counter(InitialCounter)
    {
    }
    F_TaskPromise::~F_TaskPromise()
    {
    }

    U32 F_TaskPromise::IncreaseCounter(U32 Num) noexcept
    {
        return _Counter.fetch_add(Num, boost::memory_order_release);
    }
    U32 F_TaskPromise::DecreaseCounter(U32 Num) noexcept
    {
        return _Counter.fetch_sub(Num, boost::memory_order_release);
    }
}
