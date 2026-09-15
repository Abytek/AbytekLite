#pragma once

#include "Abytek/Sandbox.NFCSamples.SimpleSRP.prerequisites.pch.hpp"
#include "Abytek/Actor/ActorComponent.hpp"


namespace Abytek
{
    class F_CanvasComponent;
    class F_CameraComponent;
    class F_InputComponent;

    class ABYTEK_SANDBOX_NFC_SAMPLES_SIMPLE_SRP_API F_SampleSpectatorComponent final : public A_ActorComponent
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ActorComponent)
        ABYTEK_END_REFLECTOR(F_SampleSpectatorComponent)
        
    private:
        TS<F_CanvasComponent> _CanvasComponent;
        TS<F_CameraComponent> _CameraComponent;
        TS<F_InputComponent> _InputComponent;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetCanvasComponent() const noexcept
        {
            return _CanvasComponent;
        }
        ABYTEK_FORCE_INLINE const auto& GetCameraComponent() const noexcept
        {
            return _CameraComponent;
        }
        ABYTEK_FORCE_INLINE const auto& GetInputComponent() const noexcept
        {
            return _InputComponent;
        }
        
    public:
        F_SampleSpectatorComponent(const F_SerializableObjectInitParams& InitParams);
        ~F_SampleSpectatorComponent() override;
        
    protected:
        void OnTick() override;
    };
}
