#include "Abytek/ActorComponents/CanvasComponentManager.hpp"
#include "Abytek/ActorComponents/CanvasComponent.hpp"
#include "Abytek/ActorComponents/RenderableComponentManager.hpp"
#include "Abytek/CoreUpdateGraph/PostTickUpdateRange.hpp"
#include "Abytek/World/WorldManager.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_CanvasComponentManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CanvasComponentManager"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_CanvasComponentManager)

    F_CanvasComponentManager::F_CanvasComponentManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
        
        AddDependency<F_WorldManager>();
        AddDependency<F_RenderableComponentManager>();
    }
    F_CanvasComponentManager::~F_CanvasComponentManager()
    {
    }

    void F_CanvasComponentManager::OnConfig()
    {
    }
    void F_CanvasComponentManager::OnPostConfig()
    {
    }

    void F_CanvasComponentManager::OnInit() 
    {
    }
    void F_CanvasComponentManager::OnStartup() 
    {
    }
    void F_CanvasComponentManager::OnTick()
    {
    }
    void F_CanvasComponentManager::OnShutdown()
    {
    }
    void F_CanvasComponentManager::OnRelease() 
    {
    }

    void F_CanvasComponentManager::DrawCanvasComponents()
    {
        auto CachedList = _CanvasComponents;
        for (const auto& CanvasComponent : CachedList)
        {
            CanvasComponent->Draw();   
        }
    }

    void F_CanvasComponentManager::_RegisterCanvasComponent(const TW_Valid<F_CanvasComponent>& Component)
    {
        _CanvasComponents.insert(Component);
        Events.OnRegisterComponent.Broadcast(Component);
    }
    void F_CanvasComponentManager::_UnregisterCanvasComponent(const TW_Valid<F_CanvasComponent>& Component)
    {
        Events.OnUnregisterComponent.Broadcast(Component);
        _CanvasComponents.erase(_CanvasComponents.find(Component));
    }
}
