#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_SLANG
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_RHISlangSubsystem final : public A_ApplicationSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_RHISlangSubsystem)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_RHISlangSubsystem);

    private:
        TF_ConcurrentStack<Slang::ComPtr<slang::IGlobalSession>> _GlobalSessions;
        U32 _Capacity;

    public:
        ABYTEK_FORCE_INLINE U32 GetCapacity() const noexcept
        {
            return _Capacity;
        }

    public:
        F_RHISlangSubsystem(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RHISlangSubsystem() override;

    protected:
        void OnInit() override;
        void OnRelease() override;

    public:
        Slang::ComPtr<slang::IGlobalSession> PopGlobalSession();
        void PushGlobalSession(const Slang::ComPtr<slang::IGlobalSession>& GlobalSession);
    };
}
#endif