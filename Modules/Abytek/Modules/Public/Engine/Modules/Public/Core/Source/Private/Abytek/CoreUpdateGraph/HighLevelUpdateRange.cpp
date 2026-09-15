#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_HighLevelUpdateRange);
    
    F_Name F_HighLevelUpdateRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::HighLevelUpdateRange::Begin");
    }
    F_Name F_HighLevelUpdateRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::HighLevelUpdateRange::End");
    }
    F_Name F_HighLevelUpdateRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::HighLevelUpdateRange");
    }

    F_HighLevelUpdateRange::F_HighLevelUpdateRange()
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
        }
    }
    F_HighLevelUpdateRange::~F_HighLevelUpdateRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_UpdateUtilities::UnregisterFunction(GetEndFunctionName());
        H_UpdateUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_HighLevelUpdateRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
    }
    void F_HighLevelUpdateRange::End()
    {
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_HighLevelUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
