#include "Abytek/CoreUpdateGraph/TickUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PrimaryUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_TickUpdateRange);
    
    F_Name F_TickUpdateRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::TickUpdateRange::Begin");
    }
    F_Name F_TickUpdateRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::TickUpdateRange::End");
    }
    F_Name F_TickUpdateRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::TickUpdateRange");
    }

    F_TickUpdateRange::F_TickUpdateRange()
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
                F_PrimaryUpdateRange::GetBeginFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_PrimaryUpdateRange::GetEndFunctionName()  
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
                F_PrimaryUpdateRange::GetEndFunctionName()  
            );
        }
    }
    F_TickUpdateRange::~F_TickUpdateRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_UpdateUtilities::UnregisterFunction(GetEndFunctionName());
        H_UpdateUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_TickUpdateRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
    }
    void F_TickUpdateRange::End()
    {
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_TickUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
