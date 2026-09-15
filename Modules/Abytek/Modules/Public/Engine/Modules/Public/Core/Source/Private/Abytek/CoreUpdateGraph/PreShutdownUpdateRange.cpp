#include "Abytek/CoreUpdateGraph/PreShutdownUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PrimaryUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_PreShutdownUpdateRange);
    
    F_Name F_PreShutdownUpdateRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::PreShutdownUpdateRange::Begin");
    }
    F_Name F_PreShutdownUpdateRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::PreShutdownUpdateRange::End");
    }
    F_Name F_PreShutdownUpdateRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::PreShutdownUpdateRange");
    }

    F_PreShutdownUpdateRange::F_PreShutdownUpdateRange()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
        {
            auto UpdateFunction = H_UpdateUtilities::RegisterFunction(
                [this]
                {
                    Begin();
                    for (const auto& Command : _Queue.PopAll())
                    {
                        Command();
                    }
                },
                GetBeginFunctionName()
            );
            UpdateFunction->AddDependency(
                F_PrimaryUpdateRange::GetEndFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_HighLevelUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_UpdateUtilities::RegisterFunction(
                [this]
                {
                    End();
                },
                GetEndFunctionName()
            );
            UpdateFunction->AddDependency(
                GetBeginFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_HighLevelUpdateRange::GetEndFunctionName()  
            );
        }
    }
    F_PreShutdownUpdateRange::~F_PreShutdownUpdateRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_UpdateUtilities::UnregisterFunction(GetEndFunctionName());
        H_UpdateUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_PreShutdownUpdateRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
    }
    void F_PreShutdownUpdateRange::End()
    {
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_PreShutdownUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
