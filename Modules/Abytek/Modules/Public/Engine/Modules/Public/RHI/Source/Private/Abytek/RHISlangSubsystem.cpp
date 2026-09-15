#include "Abytek/RHISlangSubsystem.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_SLANG
namespace Abytek
{
    ABYTEK_REFLECT(F_RHISlangSubsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RHISlangSubsystem"));
    }

    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_RHISlangSubsystem);

    F_RHISlangSubsystem::F_RHISlangSubsystem(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
    }
    F_RHISlangSubsystem::~F_RHISlangSubsystem()
    {
    }

    void F_RHISlangSubsystem::OnInit()
    {
        _Capacity = F_TaskManager::GetInstance()->GetConfig().NumLowFrequencyWorkers + 1;
    }
    void F_RHISlangSubsystem::OnRelease()
    {
    }

    Slang::ComPtr<slang::IGlobalSession> F_RHISlangSubsystem::PopGlobalSession()
    {
        Slang::ComPtr<slang::IGlobalSession> globalSession;
        if (!_GlobalSessions.TryPop(globalSession))
        {
            SlangGlobalSessionDesc desc = {};
            desc.minLanguageVersion = SLANG_LANGUAGE_VERSION_2025;
            createGlobalSession(&desc, globalSession.writeRef());
        }
        return globalSession;
    }
    void F_RHISlangSubsystem::PushGlobalSession(const Slang::ComPtr<slang::IGlobalSession>& GlobalSession)
    {
        if (_GlobalSessions.GetSize() < _Capacity)
        {
            _GlobalSessions.Push(GlobalSession);
        }
    }
}
#endif