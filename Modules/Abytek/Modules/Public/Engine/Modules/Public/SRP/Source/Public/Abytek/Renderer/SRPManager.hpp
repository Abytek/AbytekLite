#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/World/WorldSubsystem.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPManager final : public A_WorldSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldSubsystem)
        ABYTEK_END_REFLECTOR(F_SRPManager);
        
    public:
        ABYTEK_DECLARE_STATIC_SUBSYSTEM(F_SRPManager);

    private:
        
    public:

    public:
        F_SRPManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_SRPManager() override;
        
    protected:
        void OnPostConfig() override;

    protected:
        void OnStartup() override;
        void OnShutdown() override;
    };
}
