#include "Abytek/RenderBase/RenderScenePostUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PostTickUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PreShutdownUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PrimaryUpdateRange.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldManager.hpp"
#include "Abytek/RenderBase/RenderScene.hpp"
#include "Abytek/RenderBase/WorldRenderResource.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"
#include "Abytek/RenderBase/RenderSceneUpdateRange.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_RenderScenePostUpdateRange);
    
    F_Name F_RenderScenePostUpdateRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::RenderScenePostUpdateRange::Begin");
    }
    F_Name F_RenderScenePostUpdateRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::RenderScenePostUpdateRange::End");
    }
    F_Name F_RenderScenePostUpdateRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::RenderScenePostUpdateRange");
    }

    F_RenderScenePostUpdateRange::F_RenderScenePostUpdateRange()
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
                F_RenderSceneUpdateRange::GetEndFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_PreShutdownUpdateRange::GetBeginFunctionName()  
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
                F_PreShutdownUpdateRange::GetBeginFunctionName()  
            );
        }
    }
    F_RenderScenePostUpdateRange::~F_RenderScenePostUpdateRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_UpdateUtilities::UnregisterFunction(GetEndFunctionName());
        H_UpdateUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_RenderScenePostUpdateRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
        
        for (const auto& World : F_WorldManager::GetInstance()->GetWorlds())
        {
            if (auto WorldRenderResource = F_WorldRenderResource::Get_MainTask(World))
            {
                H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                    [WorldRenderResource]
                    {
                        WorldRenderResource->GetScene()->OnBeginPostUpdate();
                    }
                );
            }
        }
    }
    void F_RenderScenePostUpdateRange::End()
    {
        for (const auto& World : F_WorldManager::GetInstance()->GetWorlds())
        {
            if (auto WorldRenderResource = F_WorldRenderResource::Get_MainTask(World))
            {
                H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                    [WorldRenderResource]
                    {
                        WorldRenderResource->GetScene()->OnEndPostUpdate();
                    }
                );
            }
        }
        
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_RenderScenePostUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
