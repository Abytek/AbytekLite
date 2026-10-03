#include "Abytek/StaticMeshComponentSampleLevel.hpp"
#include "Abytek/SampleSpectatorComponent.hpp"
#include "Abytek/SampleStaticMeshComponent.hpp"
#include "Abytek/Actor/Actor.hpp"
#include "Abytek/ActorComponents/Render/CameraComponentRenderProxy.hpp"
#include "Abytek/ActorComponents/CanvasComponent.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/Assets/StaticMesh.hpp"
#include "Abytek/Assets/Render/StaticMeshRenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/Renderer/Renderer.hpp"
#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/SRPBasicDrawers/StaticMesh.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{ 
    ABYTEK_REFLECT(F_StaticMeshComponentSampleLevel)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_StaticMeshComponentSampleLevel"));
    }
    
    F_StaticMeshComponentSampleLevel::F_StaticMeshComponentSampleLevel(const F_SerializableObjectInitParams& InitParams) :
        F_Level(InitParams)
    {
    }
    F_StaticMeshComponentSampleLevel::~F_StaticMeshComponentSampleLevel()
    {
    }

    void F_StaticMeshComponentSampleLevel::OnLoadContent()
    {
        TS<F_StaticMesh> StaticMesh;
        if (
            H_WorldContext::PopulateObject<F_StaticMesh>(
                ABYTEK_WTHIS(),
                StaticMesh,
                ABYTEK_NAME("DemoStaticMesh"),
                ABYTEK_NAME("@Abytek.Sandbox.NFCSamples.StaticMeshComponent::Assets:/.IgnoreSVC/DemoStaticMesh")
            )
        )
        {
            StaticMesh->Import(ABYTEK_TEXT("@Abytek.Sandbox.NFCSamples.StaticMeshComponent::Assets:/DemoStaticMesh.fbx"));
            StaticMesh->GetPackage()->Save();
        }
        
        TS<F_Actor> SpectatorActor;
        if (
            H_WorldContext::PopulateObject<F_Actor>(
                ABYTEK_WTHIS(),
                SpectatorActor,
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
                SpectatorActor->AddOwnedComponent(SampleSpectatorComponent);
            }
            SampleSpectatorComponent->GetCameraComponent()->GetSceneComponent()->AddLocalPositionOffset(F_Vector3::Forward() * -10.0f);
        
            AddActor(SpectatorActor);
        }
        auto CameraComponent = SpectatorActor->GetComponent<F_CameraComponent>();
        
        TS<F_Actor> StaticMeshActor;
        if (
            H_WorldContext::PopulateObject<F_Actor>(
                ABYTEK_WTHIS(),
                StaticMeshActor,
                ABYTEK_NAME("SampleStaticMeshActor"),
                GetPackageName()
            )    
        )
        {
            TS<F_SampleStaticMeshComponent> SampleStaticMeshComponent;
            if (
                H_WorldContext::PopulateObject<F_SampleStaticMeshComponent>(
                    ABYTEK_WTHIS(),
                    SampleStaticMeshComponent,
                    ABYTEK_NAME("SampleStaticMeshComponent"),
                    GetPackageName()
                )    
            )
            {
                StaticMeshActor->AddOwnedComponent(SampleStaticMeshComponent);
            }
            
            AddActor(StaticMeshActor);
        }
        
        GetPackage()->Save();
    }
}
