#include "Abytek/CoreUpdateGraph/ShutdownUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PrimaryUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PreShutdownUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_ShutdownUpdateRange);
    
    F_Name F_ShutdownUpdateRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::ShutdownUpdateRange::Begin");
    }
    F_Name F_ShutdownUpdateRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::ShutdownUpdateRange::End");
    }
    F_Name F_ShutdownUpdateRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::ShutdownUpdateRange");
    }

    F_ShutdownUpdateRange::F_ShutdownUpdateRange()
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
                F_PreShutdownUpdateRange::GetEndFunctionName()
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
    F_ShutdownUpdateRange::~F_ShutdownUpdateRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_UpdateUtilities::UnregisterFunction(GetEndFunctionName());
        H_UpdateUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_ShutdownUpdateRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
    }
    void F_ShutdownUpdateRange::End()
    {
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_ShutdownUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
