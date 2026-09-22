#pragma once

#include "Abytek/Sandbox.NFCApplication.prerequisites.pch.hpp"
#include "Abytek/Level.hpp"


namespace Abytek
{
    class F_DemoLevel final : public F_Level
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_Level)
        ABYTEK_END_REFLECTOR(F_DemoLevel);
        
    public:
        F_DemoLevel(const F_SerializableObjectInitParams& InitParams);
        ~F_DemoLevel() override;
        
    protected:
        void OnLoadContent() override;
    };
}
