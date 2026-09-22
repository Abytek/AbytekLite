#pragma once

#include "Abytek/Engine.Foundation.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_FOUNDATION_API F_Application : public A_ApplicationCore
    {
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationCore)
        ABYTEK_END_REFLECTOR(F_Application);

    private:
        
    public:

    public:
        F_Application(const F_ApplicationBuildParams& BuildParams);
        ~F_Application() override;

    protected:
        void RegisterModules() override;
    };
}
