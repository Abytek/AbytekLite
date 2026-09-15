#include "Abytek/TaskContextPool.hpp"
#include "Abytek/TaskContext.hpp"


namespace Abytek
{
    F_TaskContextPool::F_TaskContextPool(Sz StackSize, U32 Capacity) :
        _StackSize(StackSize),
        _Capacity(Capacity)
    {
    }
    F_TaskContextPool::~F_TaskContextPool()
    {
    }

    TS_Unmanaged<F_TaskContext> F_TaskContextPool::Pop()
    {
        TS_Unmanaged<F_TaskContext> Context;
        if (!_Contexts.TryPop(Context))
        {
            Context = TS_Unmanaged<F_TaskContext>()(ABYTEK_WTHIS());
        }
        return ABYTEK_MOVE(Context);
    }
    void F_TaskContextPool::Push(const TS_Unmanaged<F_TaskContext>& Context)
    {
        if (_Contexts.GetSize() < _Capacity)
        {
            _Contexts.Push(Context);
        }
    }
}
