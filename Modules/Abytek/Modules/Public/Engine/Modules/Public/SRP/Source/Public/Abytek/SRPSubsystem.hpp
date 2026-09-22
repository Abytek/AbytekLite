#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/World/WorldSubsystem.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPSubsystem final : public A_WorldSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldSubsystem)
        ABYTEK_END_REFLECTOR(F_SRPSubsystem);
        
    public:
        ABYTEK_DECLARE_STATIC_SUBSYSTEM(F_SRPSubsystem);

    private:
        
    public:

    public:
        F_SRPSubsystem(const F_ProgramUnitBuildParams& BuildParams);
        ~F_SRPSubsystem() override;
        
    protected:
        void OnPostConfig() override;

    protected:
        void OnStartup() override;
        void OnShutdown() override;
    };
}
