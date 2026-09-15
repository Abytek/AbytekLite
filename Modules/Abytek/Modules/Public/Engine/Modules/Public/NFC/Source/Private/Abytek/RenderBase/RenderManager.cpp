#include "Abytek/RenderBase/RenderManager.hpp"
#include "Abytek/RenderCoreManager.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/RenderBase/RenderObjectFactory.hpp"
#include "Abytek/RenderBase/RenderPath.hpp"
#include "Abytek/RenderBase/RenderScene.hpp"
#include "Abytek/RenderBase/WorldRenderResource.hpp"
#include "Abytek/GPUData/GlobalGPUDataPack.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RenderManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RenderManager"));
    }
    
    ABYTEK_DEFINE_STATIC_SUBSYSTEM(F_RenderManager);
    
    F_RenderManager::F_RenderManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_WorldSubsystem(BuildParams)
    {
    }
    F_RenderManager::~F_RenderManager()
    {
    }

    void F_RenderManager::OnConfig()
    {
        _ConsoleVariable_RenderPathType = RegisterConsoleVariable<TF_ReflectionTypeHandle<A_RenderPath>>(
            ABYTEK_NAME("Abytek.RenderPath"),
            ABYTEK_TEXT(""),
            TF_ReflectionTypeHandle<A_RenderPath>()
        );
    }

    void F_RenderManager::OnStartup()
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
        _GPUDataPack = H_WorldContext::CreateObject<F_GlobalGPUDataPack>(GetWorld());
        SetupRenderable();
    }
    void F_RenderManager::OnShutdown()
    {
        CleanUpRenderable();
        _GPUDataPack = {};
        _RenderPath = {};
    }

    B8 F_RenderManager::IsRenderable() const
    {
        if (!A_Renderable::IsRenderable())
        {
            return false;
        }
        return GetWorld()->HasFlags(E_WorldFlag::CREATE_RENDER_SCENE);
    }

    TS<A_RenderProxy> F_RenderManager::CreateRenderProxy()
    {
        return TS<F_WorldRenderResource>()(ABYTEK_WTHIS());
    }
}
