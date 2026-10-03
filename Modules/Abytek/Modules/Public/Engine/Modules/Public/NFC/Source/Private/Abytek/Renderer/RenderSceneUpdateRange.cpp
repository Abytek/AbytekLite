#include "Abytek/Renderer/RenderSceneUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PostTickUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PreShutdownUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PrimaryUpdateRange.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldManager.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
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
                F_World::GetInitUpdateFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_World::GetStartupUpdateFunctionName()  
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
                F_World::GetShutdownUpdateFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_World::GetReleaseUpdateFunctionName()  
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
        
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            []
            {
                ABYTEK_RHI_PUSH_CAPTURE_EVENT_SCOPE_MAIN(
                    ABYTEK_DEBUG_NAME("Abytek::RenderSceneUpdate"),
                    F_Vector3_F32 { 0.25f, 1.0f, 0.75f }
                );
            }
        );
        for (const auto& World : F_WorldManager::GetInstance()->GetWorlds())
        {
            if (auto WorldRenderResource = F_WorldRenderResource::Get_MainTask(World))
            {
                H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                    [WorldRenderResource]
                    {
                        WorldRenderResource->GetScene()->OnBeginUpdate(H_RHI::GetMainSubmissionQueue());
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
                        WorldRenderResource->GetScene()->OnEndUpdate(H_RHI::GetMainSubmissionQueue());
                    }
                );
            }
        }
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            []
            {
                ABYTEK_RHI_POP_CAPTURE_EVENT_SCOPE_MAIN();
            }
        );
        
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_RenderSceneUpdateRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }
}
