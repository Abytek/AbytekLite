#include "Abytek/CoreUpdateGraph/PrimaryUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_PrimaryUpdateRange);
    
    F_Name F_PrimaryUpdateRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::PrimaryUpdateRange::Begin");
    }
    F_Name F_PrimaryUpdateRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::PrimaryUpdateRange::End");
    }
    F_Name F_PrimaryUpdateRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::PrimaryUpdateRange");
    }

    F_PrimaryUpdateRange::F_PrimaryUpdateRange()
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
    F_PrimaryUpdateRange::~F_PrimaryUpdateRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_UpdateUtilities::UnregisterFunction(GetEndFunctionName());
        H_UpdateUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_PrimaryUpdateRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
    }
    void F_PrimaryUpdateRange::End()
    {
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_PrimaryUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
