#include "Abytek/TaskContext.hpp"
#include "Abytek/Profiling.hpp"
#include "Abytek/TaskContextPool.hpp"
#include "Abytek/TaskWorker.hpp"
#include "Abytek/TaskManager.hpp"


namespace Abytek
{
    namespace Internal
    {
        AU32 NextTaskContextIndex = 0;
        thread_local F_TaskContext* CurrentTaskContextRaw;
        thread_local U32 CurrentTaskInstanceIndex;
    }
    
    F_TaskContext::F_TaskContext(const TW_Valid<F_TaskContextPool>& Pool) :
        _Pool(Pool),
        _Name(
            ABYTEK_TEXT("Abytek.TaskContext(") + ToText(Internal::NextTaskContextIndex.fetch_add(1, boost::memory_order_release)) + ABYTEK_TEXT(")")
        )
    {
        _NameString = ToString(*_Name);
        _Fiber = F_Fiber(
            [this](F_Fiber&& ParentFiber)
            {
                return _Loop(ABYTEK_MOVE(ParentFiber));
            },
            Pool->GetStackSize()
        );
        _ShouldContinue = true;
        _HasInstanceSet = false;
    }
    F_TaskContext::~F_TaskContext()
    {
        _ShouldContinue = false;
        _Fiber = ABYTEK_MOVE(_Fiber).Resume();
    }

    void F_TaskContext::_SetupTask(F_TaskInstanceSet&& InInstanceSet)
    {
        _HasInstanceSet = true;
        _InstanceSet = ABYTEK_MOVE(InInstanceSet);
        TF_Vector<F_Name> InitialTags(
            _InstanceSet.InitialTags.begin(),
            _InstanceSet.InitialTags.end()
        );
        for (U32 Idx = 0; Idx < _InstanceSet.Count; ++Idx)
        {
            _InstanceIndexToTags.push_back(InitialTags);
        }
        _BeginTaskId = F_TaskManager::GetInstance()->AcquireNextTaskId(
            InInstanceSet.Count
        );
    }
    
    F_Fiber F_TaskContext::_Loop(F_Fiber&& ParentFiber)
    {
        _ParentFiber_Temp = ABYTEK_MOVE(ParentFiber); 
        
        while (_ShouldContinue)
        {
            if (_HasInstanceSet)
            {
                _InstanceIndex = _InstanceSet.Offset;
                U32 End = _InstanceSet.Offset + _InstanceSet.Count;
                U32 Back = End - 1;
                auto Promise = _InstanceSet.Promise;
                for (; _InstanceIndex < End; ++_InstanceIndex)
                {
                    ABYTEK_PROFILER_EVENT_NAMED("Abytek::ProcessTaskInstance");
                    Internal::CurrentTaskInstanceIndex = _InstanceIndex;
                    _InstanceSet.Functor();
                    if (_InstanceIndex == Back)
                    {
                        _InstanceSet = {};
                        _InstanceIndexToTags = {};
                    }
                    Promise->DecreaseCounter(1);
                }
                _HasInstanceSet = false;
            }
            Switch();
        }
        
        return ABYTEK_MOVE(_ParentFiber_Temp);
    }
    
    void F_TaskContext::_SetThisAsCurrent()
    {
        Internal::CurrentTaskContextRaw = this;
        Internal::CurrentTaskInstanceIndex = _InstanceIndex;

#ifdef ABYTEK_ENABLE_PROFILER
        ABYTEK_PROFILER_FIBER_LEAVE();
        ABYTEK_PROFILER_FIBER_ENTER(F_TaskWorker::GetCurrent()->GetNameString().c_str());
#endif
    }
    void F_TaskContext::_UnsetThisFromCurrent()
    {
#ifdef ABYTEK_ENABLE_PROFILER
        ABYTEK_PROFILER_FIBER_LEAVE();
        ABYTEK_PROFILER_FIBER_ENTER(F_TaskWorker::GetCurrent()->GetNameString().c_str());
#endif
        
        Internal::CurrentTaskContextRaw = nullptr;
        Internal::CurrentTaskInstanceIndex = 0;
    }

    void F_TaskContext::ChangePriority(E_TaskPriority NewPriority)
    {
        _InstanceSet.Priority = NewPriority;
    }
    void F_TaskContext::Switch()
    {
        _ParentFiber_Temp = ABYTEK_MOVE(_ParentFiber_Temp).Resume();
    }
    
    F_TaskContext& F_TaskContext::GetCurrentRef()
    {
        return *Internal::CurrentTaskContextRaw;
    }
    U32 F_TaskContext::GetCurrentInstanceIndex()
    {
        return Internal::CurrentTaskInstanceIndex;
    }
}
