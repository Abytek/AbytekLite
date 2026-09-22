#include "Abytek/SRPSubsystem.hpp"
#include "Abytek/SRPRenderPath.hpp"
#include "Abytek/Renderer/RendererManager.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SRPSubsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SRPSubsystem"));
    }

    ABYTEK_DEFINE_STATIC_SUBSYSTEM(F_SRPSubsystem);

    F_SRPSubsystem::F_SRPSubsystem(const F_ProgramUnitBuildParams& BuildParams) :
        A_WorldSubsystem(BuildParams)
    {
    }
    F_SRPSubsystem::~F_SRPSubsystem()
    {
    }

    void F_SRPSubsystem::OnPostConfig()
    {
        auto RendererManager = GetContainer()->GetUnit<F_RendererManager>();
        RendererManager->GetConsoleVariable_RenderPathType()->SetValue(
            TF_ReflectionTypeHandle<F_SRPRenderPath>(F_ReflectionContext::GetGlobal())  
        );
    }

    void F_SRPSubsystem::OnStartup()
    {
    }
    void F_SRPSubsystem::OnShutdown()
    {
    }
}
