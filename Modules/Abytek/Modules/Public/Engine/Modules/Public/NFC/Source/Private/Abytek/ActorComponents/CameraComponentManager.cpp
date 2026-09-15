#include "Abytek/ActorComponents/CameraComponentManager.hpp"
#include "Abytek/ActorComponents/CameraComponent.hpp"
#include "Abytek/ActorComponents/RenderableComponentManager.hpp"
#include "Abytek/CoreUpdateGraph/PostTickUpdateRange.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_CameraComponentManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CameraComponentManager"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_CameraComponentManager)

    F_CameraComponentManager::F_CameraComponentManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
        
        AddDependency<F_RenderableComponentManager>();
    }
    F_CameraComponentManager::~F_CameraComponentManager()
    {
    }

    void F_CameraComponentManager::OnConfig()
    {
    }
    void F_CameraComponentManager::OnPostConfig()
    {
    }

    void F_CameraComponentManager::OnInit() 
    {
    }
    void F_CameraComponentManager::OnStartup() 
    {
    }
    void F_CameraComponentManager::OnTick()
    {
    }
    void F_CameraComponentManager::OnShutdown()
    {
    }
    void F_CameraComponentManager::OnRelease() 
    {
    }

    void F_CameraComponentManager::_RegisterCameraComponent(const TW_Valid<F_CameraComponent>& Component)
    {
        _CameraComponents.insert(Component);
        Events.OnRegisterComponent.Broadcast(Component);
    }
    void F_CameraComponentManager::_UnregisterCameraComponent(const TW_Valid<F_CameraComponent>& Component)
    {
        Events.OnUnregisterComponent.Broadcast(Component);
        _CameraComponents.erase(_CameraComponents.find(Component));
    }
}
