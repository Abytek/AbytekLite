#pragma once

#include "Abytek/Sandbox.EmptyApplication.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_EMPTY_APPLICATION_API F_EmptySubsystem final : public A_ApplicationSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_EmptySubsystem);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_EmptySubsystem);
        
    public:
        F_EmptySubsystem(const F_ProgramUnitBuildParams& BuildParams);
        ~F_EmptySubsystem() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
        void OnShutdown() override;
    };
}
