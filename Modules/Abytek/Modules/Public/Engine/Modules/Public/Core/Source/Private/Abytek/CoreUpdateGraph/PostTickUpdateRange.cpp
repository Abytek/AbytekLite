#include "Abytek/CoreUpdateGraph/PostTickUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PrimaryUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/TickUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_PostTickUpdateRange);
    
    F_Name F_PostTickUpdateRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::PostTickUpdateRange::Begin");
    }
    F_Name F_PostTickUpdateRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::PostTickUpdateRange::End");
    }
    F_Name F_PostTickUpdateRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::PostTickUpdateRange");
    }

    F_PostTickUpdateRange::F_PostTickUpdateRange()
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
                F_TickUpdateRange::GetEndFunctionName()
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
    F_PostTickUpdateRange::~F_PostTickUpdateRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_UpdateUtilities::UnregisterFunction(GetEndFunctionName());
        H_UpdateUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_PostTickUpdateRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
    }
    void F_PostTickUpdateRange::End()
    {
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_PostTickUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
