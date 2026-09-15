#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ApplicationCore.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_OperatorApplication : public F_Application
    {
        ABYTEK_BEGIN_REFLECTOR(F_Application)
        ABYTEK_END_REFLECTOR(F_OperatorApplication)

    public:
        F_OperatorApplication(const F_ApplicationConfig& Config = {
            ABYTEK_TEXT("Abytek::DefaultOperatorApplication")
        });
        ~F_OperatorApplication();

    protected:
        virtual B8 Init();
        virtual B8 PostInit();
        virtual B8 PreRelease();
        virtual B8 Release();
    };
}
