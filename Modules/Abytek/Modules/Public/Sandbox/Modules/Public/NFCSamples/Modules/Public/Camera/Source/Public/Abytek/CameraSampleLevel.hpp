#pragma once

#include "Abytek/Sandbox.NFCSamples.Camera.prerequisites.pch.hpp"
#include "Abytek/Level.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_CAMERA_API F_CameraSampleLevel final : public F_Level
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_Level)
        ABYTEK_END_REFLECTOR(F_CameraSampleLevel)
        
    public:
        F_CameraSampleLevel(const F_SerializableObjectInitParams& InitParams);
        ~F_CameraSampleLevel() override;
        
    protected:
        void OnLoadContent() override;
    };
}
