#pragma once

#include "Abytek/Sandbox.NFCSamples.Canvas.prerequisites.pch.hpp"
#include "Abytek/Level.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_CANVAS_API F_CanvasSampleLevel final : public F_Level
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_Level)
        ABYTEK_END_REFLECTOR(F_CanvasSampleLevel)
        
    public:
        F_CanvasSampleLevel(const F_SerializableObjectInitParams& InitParams);
        ~F_CanvasSampleLevel() override;
        
    protected:
        void OnLoadContent() override;
    };
}
