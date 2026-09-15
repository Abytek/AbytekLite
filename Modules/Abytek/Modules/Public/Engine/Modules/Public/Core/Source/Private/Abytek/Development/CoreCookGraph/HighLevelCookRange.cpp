#include "Abytek/Development/CoreCookGraph/HighLevelCookRange.hpp"
#include "Abytek/Development/Cook/CookUtilities.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_HighLevelCookRange);
    
    F_Name F_HighLevelCookRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::HighLevelCookRange::Begin");
    }
    F_Name F_HighLevelCookRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::HighLevelCookRange::End");
    }
    F_Name F_HighLevelCookRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::HighLevelCookRange");
    }

    F_HighLevelCookRange::F_HighLevelCookRange()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
        {
            auto CookFunction = H_CookUtilities::RegisterFunction(
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
            auto CookFunction = H_CookUtilities::RegisterFunction(
                [this]
                {
                    End();
                },
                GetEndFunctionName()
            );
            CookFunction->AddDependency(
                GetBeginFunctionName()
            );
        }
    }
    F_HighLevelCookRange::~F_HighLevelCookRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_CookUtilities::UnregisterFunction(GetEndFunctionName());
        H_CookUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_HighLevelCookRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
    }
    void F_HighLevelCookRange::End()
    {
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_HighLevelCookRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
#endif