#include "Abytek/RenderBase/RenderSceneUpdateRange.hpp"
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


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_RenderSceneUpdateRange);
    
    F_Name F_RenderSceneUpdateRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::RenderSceneUpdateRange::Begin");
    }
    F_Name F_RenderSceneUpdateRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::RenderSceneUpdateRange::End");
    }
    F_Name F_RenderSceneUpdateRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::RenderSceneUpdateRange");
    }

    F_RenderSceneUpdateRange::F_RenderSceneUpdateRange()
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
                F_PrimaryUpdateRange::GetEndFunctionName()
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
    F_RenderSceneUpdateRange::~F_RenderSceneUpdateRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_UpdateUtilities::UnregisterFunction(GetEndFunctionName());
        H_UpdateUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_RenderSceneUpdateRange::Begin()
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
                        WorldRenderResource->GetScene()->OnBeginUpdate();
                    }
                );
            }
        }
    }
    void F_RenderSceneUpdateRange::End()
    {
        for (const auto& World : F_WorldManager::GetInstance()->GetWorlds())
        {
            if (auto WorldRenderResource = F_WorldRenderResource::Get_MainTask(World))
            {
                H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                    [WorldRenderResource]
                    {
                        WorldRenderResource->GetScene()->OnEndUpdate();
                    }
                );
            }
        }
        
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_RenderSceneUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
