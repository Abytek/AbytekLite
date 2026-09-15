#include "Abytek/Actor/ActorComponentManager.hpp"
#include "Abytek/Actor/ActorComponent.hpp"
#include "Abytek/World/WorldManager.hpp"
#include "Abytek/CoreUpdateGraph/PostTickUpdateRange.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_ActorComponentManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ActorComponentManager"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_ActorComponentManager)

    F_ActorComponentManager::F_ActorComponentManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
        
        AddDependency<F_WorldManager>();
    }
    F_ActorComponentManager::~F_ActorComponentManager()
    {
    }

    void F_ActorComponentManager::OnConfig()
    {
    }
    void F_ActorComponentManager::OnPostConfig()
    {
    }

    void F_ActorComponentManager::OnInit() 
    {
        A_ActorComponent::GlobalInit();
    }
    void F_ActorComponentManager::OnStartup() 
    {
    }
    void F_ActorComponentManager::OnTick()
    {
    }
    void F_ActorComponentManager::OnShutdown()
    {
    }
    void F_ActorComponentManager::OnRelease() 
    {
        A_ActorComponent::GlobalRelease();
    }

    void F_ActorComponentManager::_RegisterActorComponentToTick(const TW_Valid<A_ActorComponent>& Component)
    {
        _ActorComponentsToTick.insert(Component);
        Events.OnRegisterComponentToTick.Broadcast(Component);
    }
    void F_ActorComponentManager::_UnregisterActorComponentFromTick(const TW_Valid<A_ActorComponent>& Component)
    {
        Events.OnUnregisterComponentFromTick.Broadcast(Component);
        _ActorComponentsToTick.erase(_ActorComponentsToTick.find(Component));
    }

    void F_ActorComponentManager::_RegisterActorComponentToPostTick(const TW_Valid<A_ActorComponent>& Component)
    {
        _ActorComponentsToPostTick.insert(Component);
        Events.OnRegisterComponentToPostTick.Broadcast(Component);
    }
    void F_ActorComponentManager::_UnregisterActorComponentFromPostTick(const TW_Valid<A_ActorComponent>& Component)
    {
        Events.OnUnregisterComponentFromPostTick.Broadcast(Component);
        _ActorComponentsToPostTick.erase(_ActorComponentsToPostTick.find(Component));
    }

    void F_ActorComponentManager::Tick()
    {
        auto CachedList = _ActorComponentsToTick;
        for (const auto& ActorComponent : CachedList)
        {
            ActorComponent->OnTick();
        }
    }
    void F_ActorComponentManager::PostTick()
    {
        auto CachedList = _ActorComponentsToPostTick;
        for (const auto& ActorComponent : CachedList)
        {
            ActorComponent->OnPostTick();
        }
    }
}
