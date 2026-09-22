#pragma once

#include "Abytek/Engine.MFC.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_MFC_API F_MFCModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_MFCModule);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_MFCModule);
        
    public:
        F_MFCModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_MFCModule() override;
    };
}
