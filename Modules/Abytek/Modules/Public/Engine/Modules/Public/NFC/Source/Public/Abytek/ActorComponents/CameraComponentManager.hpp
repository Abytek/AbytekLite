#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    class F_CameraComponent;
    
    namespace CameraComponentManagerEvents
    {
        using F_OnRegisterComponent = TF_Event<void(const TW_Valid<F_CameraComponent>& Component)>;
        using F_OnUnregisterComponent = TF_Event<void(const TW_Valid<F_CameraComponent>& Component)>;
    }
    struct F_CameraComponentManagerEvents
    {
        CameraComponentManagerEvents::F_OnRegisterComponent OnRegisterComponent;
        CameraComponentManagerEvents::F_OnUnregisterComponent OnUnregisterComponent;
    };
    
    class ABYTEK_ENGINE_NFC_API F_CameraComponentManager final : public A_ApplicationSubsystem
    {
    public:
        friend class F_CameraComponent;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_CameraComponentManager)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_CameraComponentManager);

    private:
        TF_Set<TW<F_CameraComponent>> _CameraComponents;
        
    public:
        F_CameraComponentManagerEvents Events;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetCameraComponents() const noexcept
        {
            return _CameraComponents;
        }
        
    public:
        F_CameraComponentManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_CameraComponentManager() override;

    protected:
        void OnConfig() override;
        void OnPostConfig() override;
        
    protected:
        void OnInit() override;
        void OnStartup() override;
        void OnTick() override;
        void OnShutdown() override;
        void OnRelease() override;
        
    private:
        void _RegisterCameraComponent(const TW_Valid<F_CameraComponent>& Component);
        void _UnregisterCameraComponent(const TW_Valid<F_CameraComponent>& Component);
    };
}
