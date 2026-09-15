#pragma once

#include "Abytek/Engine.VMM.prerequisites.hpp"
#include "Abytek/ApplicationSubsystem.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_VMM_API F_VMMSubsystem final : public A_ApplicationSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_VMMSubsystem)

    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_VMMSubsystem);

    private:
        
    public:

    public:
        F_VMMSubsystem(const F_ProgramUnitBuildParams& BuildParams);
        ~F_VMMSubsystem() override;
    };
}
