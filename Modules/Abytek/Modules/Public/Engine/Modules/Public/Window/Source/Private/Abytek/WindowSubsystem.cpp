#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/WindowSubsystemProxy.hpp"
#include "Abytek/WindowManager.hpp"
#include "Abytek/CursorManager.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_WindowSubsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_WindowSubsystem"));
    }

    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_WindowSubsystem);

    F_WindowSubsystem::F_WindowSubsystem(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
    }
    F_WindowSubsystem::~F_WindowSubsystem()
    {
    }

    void F_WindowSubsystem::OnInit()
    {
        _APIs = WindowGetDefaultAPIs();

        _Proxy = RACreateAndBuild<A_WindowSubsystemProxy>();
        _WindowManager = RACreateAndBuild<A_WindowManager>();
        _CursorManager = RACreateAndBuild<A_CursorManager>();
    }
    void F_WindowSubsystem::OnStartup()
    {
        _WindowManager->Startup();
        _CursorManager->Startup();
    }
    void F_WindowSubsystem::OnShutdown()
    {
        _CursorManager->Shutdown();
        _WindowManager->Shutdown();
    }
    void F_WindowSubsystem::OnRelease()
    {
        _CursorManager = {};
        _WindowManager = {};
        _Proxy = {};
    }
}
