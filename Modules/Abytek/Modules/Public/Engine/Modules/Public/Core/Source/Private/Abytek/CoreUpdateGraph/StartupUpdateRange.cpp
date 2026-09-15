#include "Abytek/CoreUpdateGraph/StartupUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PrimaryUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_StartupUpdateRange);
    
    F_Name F_StartupUpdateRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::StartupUpdateRange::Begin");
    }
    F_Name F_StartupUpdateRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::StartupUpdateRange::End");
    }
    F_Name F_StartupUpdateRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::StartupUpdateRange");
    }

    F_StartupUpdateRange::F_StartupUpdateRange()
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
                F_HighLevelUpdateRange::GetBeginFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_PrimaryUpdateRange::GetBeginFunctionName()  
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
                F_PrimaryUpdateRange::GetBeginFunctionName()  
            );
        }
    }
    F_StartupUpdateRange::~F_StartupUpdateRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_UpdateUtilities::UnregisterFunction(GetEndFunctionName());
        H_UpdateUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_StartupUpdateRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
    }
    void F_StartupUpdateRange::End()
    {
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_StartupUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
