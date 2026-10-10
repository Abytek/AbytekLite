#pragma once

#include "Abytek/Sandbox.NFCSamples.StandardMaterial.prerequisites.pch.hpp"
#include "Abytek/Level/Level.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_STANDARD_MATERIAL_API F_StandardMaterialSampleLevel final : public F_Level
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_Level)
        ABYTEK_END_REFLECTOR(F_StandardMaterialSampleLevel);
        
    public:
        F_StandardMaterialSampleLevel(const F_SerializableObjectInitParams& InitParams);
        ~F_StandardMaterialSampleLevel() override;
        
    protected:
        void OnLoadContent() override;
    };
}
