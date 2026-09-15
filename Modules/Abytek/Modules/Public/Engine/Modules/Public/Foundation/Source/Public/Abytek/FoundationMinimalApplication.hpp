#pragma once

#include "Abytek/Engine.Foundation.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_FOUNDATION_API F_MinimalApplication : public A_ApplicationCore
    {
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationCore)
        ABYTEK_END_REFLECTOR(F_MinimalApplication)

    private:
        
    public:

    public:
        F_MinimalApplication(const F_ApplicationBuildParams& BuildParams);
        ~F_MinimalApplication() override;

    protected:
        void RegisterModules() override;
    };
}
