#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ApplicationCore.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_ANodeApplication : public F_Application
    {
        ABYTEK_BEGIN_REFLECTOR(F_Application)
        ABYTEK_END_REFLECTOR(F_ANodeApplication)

    public:
        F_ANodeApplication(const F_ApplicationConfig& Config = {
            ABYTEK_TEXT("Abytek::DefaultANodeApplication")
        });
        ~F_ANodeApplication();

    protected:
        virtual B8 Init();
        virtual B8 PostInit();
        virtual B8 PreRelease();
        virtual B8 Release();
    };
}
