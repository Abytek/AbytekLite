#include "Abytek/ActorComponents/RenderableComponentUpdateRange.hpp"
#include "Abytek/RenderBase/RenderSceneUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_RenderableComponentUpdateRange);
    
    F_Name F_RenderableComponentUpdateRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::RenderableComponentUpdateRange::Begin");
    }
    F_Name F_RenderableComponentUpdateRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::RenderableComponentUpdateRange::End");
    }
    F_Name F_RenderableComponentUpdateRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::RenderableComponentUpdateRange");
    }

    F_RenderableComponentUpdateRange::F_RenderableComponentUpdateRange()
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
                F_RenderSceneUpdateRange::GetBeginFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_RenderSceneUpdateRange::GetEndFunctionName()  
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
                F_RenderSceneUpdateRange::GetEndFunctionName() 
            );
        }
    }
    F_RenderableComponentUpdateRange::~F_RenderableComponentUpdateRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_UpdateUtilities::UnregisterFunction(GetEndFunctionName());
        H_UpdateUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_RenderableComponentUpdateRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
    }
    void F_RenderableComponentUpdateRange::End()
    {
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_RenderableComponentUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
