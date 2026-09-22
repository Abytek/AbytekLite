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

    void F_RendererManager::OnStartup()
    {
        {
            auto RenderPathType = _ConsoleVariable_RenderPathType->GetValue();
            ABYTEK_ENGINE_NFC_ASSERT(RenderPathType) << "Invalid render path type";
            
            _RenderPath = H_WorldContext::CreateObject<A_RenderPath>(
                GetWorld(),
                {},
                {},
                RenderPathType
            );
        }
        SetupRenderable();
    }
    void F_RendererManager::OnShutdown()
    {
        CleanUpRenderable();
        _RenderPath = {};
    }

    B8 F_RendererManager::IsRenderable() const
    {
        if (!A_Renderable::IsRenderable())
        {
            return false;
        }
        return GetWorld()->HasFlags(E_WorldFlag::CREATE_RENDER_SCENE);
    }

    TS<A_RenderProxy> F_RendererManager::CreateRenderProxy()
    {
        return TS<F_WorldRenderResource>()(ABYTEK_WTHIS());
    }
}
