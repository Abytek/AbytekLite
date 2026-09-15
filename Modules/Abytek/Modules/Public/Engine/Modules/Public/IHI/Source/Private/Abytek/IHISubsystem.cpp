#include "Abytek/IHISubsystem.hpp"
#include "Abytek/IHISubsystemProxy.hpp"
#include "Abytek/IHIMouseManager.hpp"
#include "Abytek/IHIKeyboardManager.hpp"
#include "Abytek/IHIButtonCoder.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_IHISubsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_IHISubsystem"));
    }

    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_IHISubsystem);

    F_IHISubsystem::F_IHISubsystem(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
    }
    F_IHISubsystem::~F_IHISubsystem()
    {
    }

    void F_IHISubsystem::OnInit()
    {
        _APIs = IHIGetDefaultAPIs();

        _Proxy = RACreateAndBuild<A_IHISubsystemProxy>();
        _ButtonCoder = RACreateAndBuild<A_IHIButtonCoder>();
        _MouseManager = RACreateAndBuild<A_IHIMouseManager>();
        _KeyboardManager = RACreateAndBuild<A_IHIKeyboardManager>();
    }
    void F_IHISubsystem::OnStartup()
    {
        _MouseManager->Startup();
        _KeyboardManager->Startup();
    }
    void F_IHISubsystem::OnShutdown()
    {
        _KeyboardManager->Shutdown();
        _MouseManager->Shutdown();
    }
    void F_IHISubsystem::OnRelease()
    {
        _KeyboardManager.Reset();
        _MouseManager.Reset();
        _ButtonCoder.Reset();
        _Proxy.Reset();
    }
}
