#include "Abytek/SimpleSRPSampleLevel.hpp"
#include "Abytek/SampleSpectatorComponent.hpp"
#include "Abytek/Actor/Actor.hpp"
#include "Abytek/ActorComponents/Render/CameraRenderProxy.hpp"
#include "Abytek/ActorComponents/CanvasComponent.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/Assets/StaticMesh.hpp"
#include "Abytek/Assets/Render/StaticMeshRenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/RenderBase/Renderer.hpp"
#include "Abytek/RenderBase/RenderViewFamily.hpp"
#include "Abytek/RenderBase/RenderView.hpp"
#include "Abytek/RenderBase/WorldRenderResource.hpp"
#include "Abytek/SRPBasicDrawers/StaticMesh.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{ 
    ABYTEK_REFLECT(F_SimpleSRPSampleLevel)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SimpleSRPSampleLevel"));
    }
    
    F_SimpleSRPSampleLevel::F_SimpleSRPSampleLevel(const F_SerializableObjectInitParams& InitParams) :
        F_Level(InitParams)
    {
    }
    F_SimpleSRPSampleLevel::~F_SimpleSRPSampleLevel()
    {
    }

    void F_SimpleSRPSampleLevel::OnLoadContent()
    {
        TS<F_StaticMesh> StaticMesh;
        if (
            H_WorldContext::PopulateObject<F_StaticMesh>(
                ABYTEK_WTHIS(),
                StaticMesh,
                ABYTEK_NAME("DemoStaticMesh"),
                ABYTEK_NAME("@Abytek.Sandbox.NFCSamples.SimpleSRP::Assets:/.IgnoreSVC/DemoStaticMesh")
            )
        )
        {
            StaticMesh->Import(ABYTEK_TEXT("@Abytek.Sandbox.NFCSamples.SimpleSRP::Assets:/DemoStaticMesh.fbx"));
            StaticMesh->GetPackage()->Save();
        }
        
        TS<F_Actor> Actor;
        if (
            H_WorldContext::PopulateObject<F_Actor>(
                ABYTEK_WTHIS(),
                Actor,
                ABYTEK_NAME("SampleSpectatorActor"),
                GetPackageName()
            )    
        )
        {
            TS<F_SampleSpectatorComponent> SampleSpectatorComponent;
            if (
                H_WorldContext::PopulateObject<F_SampleSpectatorComponent>(
                    ABYTEK_WTHIS(),
                    SampleSpectatorComponent,
                    ABYTEK_NAME("SampleSpectatorComponent"),
                    GetPackageName()
                )    
            )
            {
                Actor->AddOwnedComponent(SampleSpectatorComponent);
            }
            
            AddActor(Actor);
        }
        auto CameraComponent = Actor->GetComponent<F_CameraComponent>();
        GetPackage()->Save();
        
        H_UpdateUtilities::GetFunction(
            F_CanvasComponent::GetPreDrawUpdateFunctionName()    
        )->Event.AddListener(
            [=]
            {
                auto WorldRenderResource = F_WorldRenderResource::Get_MainTask(ABYTEK_WTHIS());
                auto StaticMeshRenderProxy = StaticMesh->GetRenderProxy().StaticCast<F_StaticMeshRenderProxy>();
                auto CameraRenderProxy = CameraComponent->GetRenderProxy().StaticCast<F_CameraRenderProxy>();
                
                H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                    [=]
                    {
                        auto Renderer = CameraRenderProxy->GetRenderer();
                        Renderer->PostRenderQueue.Push(
                            [=]
                            {
                                auto SubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(H_RHI::GetMainProcess().Weak());
                                for (const auto& View : Renderer->GetViewFamily()->GetViews())
                                {
                                    H_SRPStaticMeshDrawer::Render(
                                        *SubmissionList,
                                        View.Weak(),
                                        StaticMeshRenderProxy.Weak(),
                                        MakeTranslationMatrix(F_Vector3_F32(0.0f, 0.0f, 5.0f)),
                                        F_Vector4_F32 { 0.0f, 1.0f, 1.0f, 1.0f },
                                        E_RHIFillMode::SOLID
                                    );
                                }
                                H_RHI::GetMainProcess()->AddSubmissionItem(SubmissionList);
                            }
                        );
                    }
                );
            }
        );
    }
}
