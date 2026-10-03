#include "Abytek/SampleStaticMeshComponent.hpp"
#include "Abytek/Actor/Actor.hpp"
#include "Abytek/World/WorldContextHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SampleStaticMeshComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SampleStaticMeshComponent"));
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_StaticMeshComponent);
    }
    
    F_SampleStaticMeshComponent::F_SampleStaticMeshComponent(const F_SerializableObjectInitParams& InitParams) :
        A_ActorComponent(InitParams)
    {
        _StaticMeshComponent = CreateSerializableSubobjectDelayLoading<F_StaticMeshComponent>(
            ABYTEK_NAME("StaticMeshComponent")
        );
        _StaticMeshComponent->SetStaticMesh(
            H_WorldContext::CreateObjectDelayLoading<F_StaticMesh>(
                ABYTEK_WTHIS(),
                ABYTEK_NAME("DemoStaticMesh"),
                ABYTEK_NAME("@Abytek.Sandbox.NFCSamples.StaticMeshComponent::Assets:/.IgnoreSVC/DemoStaticMesh")
            )
        );
        AddChildInstanceComponent(_StaticMeshComponent);
        
        EnableTick();
    }
    F_SampleStaticMeshComponent::~F_SampleStaticMeshComponent()
    {
    }

    void F_SampleStaticMeshComponent::OnTick()
    {
    }
}
