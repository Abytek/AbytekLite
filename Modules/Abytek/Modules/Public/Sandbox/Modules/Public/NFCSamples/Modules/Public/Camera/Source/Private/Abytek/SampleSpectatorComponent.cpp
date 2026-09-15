#include "Abytek/SampleSpectatorComponent.hpp"
#include "Abytek/Actor/Actor.hpp"
#include "Abytek/ActorComponents/CanvasComponent.hpp"
#include "Abytek/ActorComponents/CameraComponent.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SampleSpectatorComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SampleSpectatorComponent"));
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_CanvasComponent);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_CameraComponent);
    }
    
    F_SampleSpectatorComponent::F_SampleSpectatorComponent(const F_SerializableObjectInitParams& InitParams) :
        A_ActorComponent(InitParams)
    {
        _CanvasComponent = CreateDefaultSerializableSubobject<F_CanvasComponent>(
            ABYTEK_NAME("CanvasComponent")    
        );
        AddChildInstanceComponent(_CanvasComponent);
        
        _CameraComponent = CreateDefaultSerializableSubobject<F_CameraComponent>(
            ABYTEK_NAME("CameraComponent")    
        );
        AddChildInstanceComponent(_CameraComponent);
    }
    F_SampleSpectatorComponent::~F_SampleSpectatorComponent()
    {
    }
}
