#pragma once

#include "Abytek/Sandbox.EmptyApplication.prerequisites.pch.hpp"


namespace Abytek
{
    class F_EmptyApplication : public F_Application
    {
        ABYTEK_BEGIN_REFLECTOR(F_Application)
        ABYTEK_END_REFLECTOR(F_EmptyApplication);

    private:
        
    public:

    public:
        F_EmptyApplication(const F_ApplicationBuildParams& BuildParams);
        ~F_EmptyApplication() override;

    protected:
        void Init() override;
    };
}
