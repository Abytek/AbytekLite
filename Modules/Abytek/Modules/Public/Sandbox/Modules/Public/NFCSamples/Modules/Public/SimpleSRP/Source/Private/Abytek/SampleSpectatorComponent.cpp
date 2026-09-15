#include "Abytek/SampleSpectatorComponent.hpp"

#include "Abytek/GlobalTime.hpp"
#include "Abytek/Actor/Actor.hpp"
#include "Abytek/ActorComponents/CanvasComponent.hpp"
#include "Abytek/ActorComponents/CameraComponent.hpp"
#include "Abytek/ActorComponents/InputComponent.hpp"
#include "Abytek/ActorComponents/SceneComponent.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SampleSpectatorComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SampleSpectatorComponent"));
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_CanvasComponent);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_CameraComponent);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_InputComponent);
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
        _CameraComponent->SetProjectionVerticalFOV(60.0f * ABYTEK_F32_DEGREES_TO_RADS);
        AddChildInstanceComponent(_CameraComponent);
        
        _InputComponent = CreateDefaultSerializableSubobject<F_InputComponent>(
            ABYTEK_NAME("InputComponent")    
        );
        AddChildInstanceComponent(_InputComponent);
        
        EnableTick();
    }
    F_SampleSpectatorComponent::~F_SampleSpectatorComponent()
    {
    }

    void F_SampleSpectatorComponent::OnTick()
    {
        auto CameraSceneComponent = _CameraComponent->GetSceneComponent();
        F32 Distance = F_GlobalTime::GetInstance()->GetLastDeltaSeconds();
        F32 MouseSenitivity = 0.2f;
        if (_InputComponent->GetDeltaMousePosition().X)
        {
            CameraSceneComponent->AddWorldRotationOffset({
                0.0f,
                static_cast<F32>(_InputComponent->GetDeltaMousePosition().X) * ABYTEK_F32_DEGREES_TO_RADS * MouseSenitivity,
                0.0f
            });
        }
        if (
            _InputComponent->IsButtonPressed(E_IHIPredefinedButton::SPACE)
        )
        {
            CameraSceneComponent->AddWorldPositionOffset(
                CameraSceneComponent->GetWorldUp() * Distance
            );
        }
        if (
            _InputComponent->IsButtonPressed(E_IHIPredefinedButton::CONTROL)
        )
        {
            CameraSceneComponent->AddWorldPositionOffset(
                CameraSceneComponent->GetWorldDown() * Distance
            );
        }
        if (
            _InputComponent->IsButtonPressed(E_IHIPredefinedButton::LEFT)
            || _InputComponent->IsButtonPressed(E_IHIPredefinedButton::A)
        )
        {
            CameraSceneComponent->AddWorldPositionOffset(
                CameraSceneComponent->GetWorldLeft() * Distance
            );
        }
        if (
            _InputComponent->IsButtonPressed(E_IHIPredefinedButton::RIGHT)
            || _InputComponent->IsButtonPressed(E_IHIPredefinedButton::D)
        )
        {
            CameraSceneComponent->AddWorldPositionOffset(
                CameraSceneComponent->GetWorldRight() * Distance
            );
        }
        if (
            _InputComponent->IsButtonPressed(E_IHIPredefinedButton::UP)
            || _InputComponent->IsButtonPressed(E_IHIPredefinedButton::W)
        )
        {
            CameraSceneComponent->AddWorldPositionOffset(
                CameraSceneComponent->GetWorldForward() * Distance
            );
        }
        if (
            _InputComponent->IsButtonPressed(E_IHIPredefinedButton::DOWN)
            || _InputComponent->IsButtonPressed(E_IHIPredefinedButton::S)
        )
        {
            CameraSceneComponent->AddWorldPositionOffset(
                CameraSceneComponent->GetWorldBackward() * Distance
            );
        }
    }
}
