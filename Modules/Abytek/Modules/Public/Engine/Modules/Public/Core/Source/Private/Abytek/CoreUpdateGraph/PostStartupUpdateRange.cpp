#include "Abytek/CoreUpdateGraph/PostStartupUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/StartupUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PrimaryUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_PostStartupUpdateRange);
    
    F_Name F_PostStartupUpdateRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::PostStartupUpdateRange::Begin");
    }
    F_Name F_PostStartupUpdateRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::PostStartupUpdateRange::End");
    }
    F_Name F_PostStartupUpdateRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::PostStartupUpdateRange");
    }

    F_PostStartupUpdateRange::F_PostStartupUpdateRange()
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
                F_StartupUpdateRange::GetEndFunctionName()
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
    F_PostStartupUpdateRange::~F_PostStartupUpdateRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_UpdateUtilities::UnregisterFunction(GetEndFunctionName());
        H_UpdateUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_PostStartupUpdateRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
    }
    void F_PostStartupUpdateRange::End()
    {
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_PostStartupUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
