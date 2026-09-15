#include "Abytek/ActorComponents/RenderableComponentManager.hpp"
#include "Abytek/ActorComponents/RenderableComponent.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RenderableComponentManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RenderableComponentManager"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_RenderableComponentManager)

    F_RenderableComponentManager::F_RenderableComponentManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
    }
    F_RenderableComponentManager::~F_RenderableComponentManager()
    {
    }

    void F_RenderableComponentManager::OnConfig()
    {
    }
    void F_RenderableComponentManager::OnPostConfig()
    {
    }

    void F_RenderableComponentManager::OnInit() 
    {
    }
    void F_RenderableComponentManager::OnStartup() 
    {
    }
    void F_RenderableComponentManager::OnTick()
    {
    }
    void F_RenderableComponentManager::OnShutdown()
    {
    }
    void F_RenderableComponentManager::OnRelease() 
    {
    }

    void F_RenderableComponentManager::_RegisterRenderableComponent(const TW_Valid<A_RenderableComponent>& Component)
    {
        _RenderableComponents.insert(Component);
    }
    void F_RenderableComponentManager::_UnregisterRenderableComponent(const TW_Valid<A_RenderableComponent>& Component)
    {
        _RenderableComponents.erase(_RenderableComponents.find(Component));
    }

    void F_RenderableComponentManager::_RegisterRenderableComponentToUpdateRenderStateEveryFrame(const TW_Valid<A_RenderableComponent>& Component)
    {
        _RenderableComponentsToUpdateRenderStateEveryFrame.insert(Component);
    }
    void F_RenderableComponentManager::_UnregisterRenderableComponentFromUpdateRenderStateEveryFrame(const TW_Valid<A_RenderableComponent>& Component)
    {
        _RenderableComponentsToUpdateRenderStateEveryFrame.erase(_RenderableComponentsToUpdateRenderStateEveryFrame.find(Component));
    }

    void F_RenderableComponentManager::UpdateRenderStates()
    {
        auto CachedList = _RenderableComponentsToUpdateRenderStateEveryFrame;
        for (const auto& Component : CachedList)
        {
            Component->_UpdateRenderState();
        }
    }
}
