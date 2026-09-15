#include "Abytek/TaskLoop.hpp"
#include "Abytek/TaskUtilities.hpp"


namespace Abytek
{
    A_TaskLoop::A_TaskLoop(const TW_Valid<F_TaskWorker>& Worker, E_TaskPriority Priority, const F_Name& Name) :
        _Worker(Worker),
        _Priority(Priority),
        _Name(Name)
    {
    }
    A_TaskLoop::~A_TaskLoop()
    {
        Join();
    }

    void A_TaskLoop::Start()
    {
        _Promise = H_TaskUtilities::Schedule(
            _Worker,
            [this]()
            {
                Bind();
                while (ShouldContinue())
                {
                    Tick();
                    H_TaskUtilities::Switch();
                }
                Unbind();
            },
            _Priority,
            _Name
        );
    }
    void A_TaskLoop::SignalStop()
    {
        _ShouldContinue.store(false, boost::memory_order_release);
    }
    void A_TaskLoop::Join()
    {
        if (!IsStarted())
        {
            return;
        }
        SignalStop();
        while (!IsStopped())
        {
            H_TaskUtilities::Switch();
        }
    }

    void A_TaskLoop::Bind()
    {
        _IsStarted.store(true, boost::memory_order_release);
    }
    void A_TaskLoop::Tick()
    {
    }
    void A_TaskLoop::Unbind()
    {
        _IsStopped.store(true, boost::memory_order_release);
    }
}
