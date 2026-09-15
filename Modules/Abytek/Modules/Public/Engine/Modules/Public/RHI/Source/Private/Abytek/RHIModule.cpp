#include "Abytek/RHIModule.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHISlangSubsystem.hpp"
#include "Abytek/RHIFormat.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RHIModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RHIModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_RHIModule)
    
    F_RHIModule::F_RHIModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
        AddDependency<F_AssetsModule>();
        AddDependency<F_WindowModule>();
    }
    F_RHIModule::~F_RHIModule()
    {
    }

    void F_RHIModule::OnReflect()
    {
        RegisterStaticType<F_RHISubsystem>();
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        RegisterStaticType<F_RHISlangSubsystem>();
#endif
        RegisterStaticType<E_RHIFormat>();
    }
    void F_RHIModule::OnConfig()
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
        _ConsoleVariable_ShouldEnablePIX = RegisterConsoleVariable<B8>(
            ABYTEK_NAME("Abytek.PIX.Enable"),
            ABYTEK_TEXT(""),
            false
        );
#endif
    }
    void F_RHIModule::OnPostConfig()
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
        _ShouldEnablePIX = _ConsoleVariable_ShouldEnablePIX->GetValue();
#endif
    }
}
