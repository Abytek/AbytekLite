#include "Abytek/Renderer/RendererManager.hpp"
#include "Abytek/RenderCoreManager.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"
#include "Abytek/Renderer/RenderPath.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_WorldRenderResourceOwner)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_WorldRenderResourceOwner"));
    }
    
    F_WorldRenderResourceOwner::F_WorldRenderResourceOwner(const F_SerializableObjectInitParams& InitParams) :
        A_WorldContext(InitParams)
    {
        _Manager = H_WorldContext::GetUnit<F_RendererManager>(ABYTEK_WTHIS());
    }
    F_WorldRenderResourceOwner::~F_WorldRenderResourceOwner()
    {
    }

    void F_WorldRenderResourceOwner::OnLoad()
    {
        SetupRenderable();
    }
    void F_WorldRenderResourceOwner::OnUnload()
    {
        CleanUpRenderable();
    }

    TS<A_RenderProxy> F_WorldRenderResourceOwner::CreateRenderProxy()
    {
        return TS<F_WorldRenderResource>()(ABYTEK_WTHIS());
    }

    ABYTEK_REFLECT(F_RendererManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RendererManager"));
    }
    
    ABYTEK_DEFINE_STATIC_SUBSYSTEM(F_RendererManager);
    
    F_RendererManager::F_RendererManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_WorldSubsystem(BuildParams)
    {
    }
    F_RendererManager::~F_RendererManager()
    {
    }

    void F_RendererManager::OnConfig()
    {
        _ConsoleVariable_RenderPathType = RegisterConsoleVariable<TF_ReflectionTypeHandle<A_RenderPath>>(
            ABYTEK_NAME("Abytek.RenderPath"),
            ABYTEK_TEXT(""),
            TF_ReflectionTypeHandle<A_RenderPath>()
        );
    }

    void F_RendererManager::OnInit()
    {
        {
            auto RenderPathType = _ConsoleVariable_RenderPathType->GetValue();
            ABYTEK_ENGINE_NFC_ASSERT(RenderPathType) << "Invalid render path type";
            _RenderPath = H_WorldContext::CreateObjectDelayLoading<A_RenderPath>(
                GetWorld(),
                {},
                {},
                RenderPathType
            );
        }
        {
            _WorldRenderResourceOwner = H_WorldContext::CreateObjectDelayLoading<F_WorldRenderResourceOwner>(
                GetWorld()  
            );
            // Force load
            _WorldRenderResourceOwner->CallLoad();
        }
    }
    void F_RendererManager::OnStartup()
    {
    }
    void F_RendererManager::OnShutdown()
    {
    }
    void F_RendererManager::OnRelease()
    {
        _WorldRenderResourceOwner = {};
        _RenderPath = {};
    }
}
