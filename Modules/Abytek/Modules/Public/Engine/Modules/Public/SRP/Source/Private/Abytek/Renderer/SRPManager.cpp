#include "Abytek/Renderer/SRPManager.hpp"
#include "Abytek/Renderer/SRPRenderPath.hpp"
#include "Abytek/Renderer/RendererManager.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SRPManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SRPManager"));
    }

    ABYTEK_DEFINE_STATIC_SUBSYSTEM(F_SRPManager);

    F_SRPManager::F_SRPManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_WorldSubsystem(BuildParams)
    {
    }
    F_SRPManager::~F_SRPManager()
    {
    }

    void F_SRPManager::OnPostConfig()
    {
        auto RendererManager = GetContainer()->GetUnit<F_RendererManager>();
        RendererManager->GetConsoleVariable_RenderPathType()->SetValue(
            TF_ReflectionTypeHandle<F_SRPRenderPath>(F_ReflectionContext::GetGlobal())  
        );
    }

    void F_SRPManager::OnStartup()
    {
    }
    void F_SRPManager::OnShutdown()
    {
    }
}
