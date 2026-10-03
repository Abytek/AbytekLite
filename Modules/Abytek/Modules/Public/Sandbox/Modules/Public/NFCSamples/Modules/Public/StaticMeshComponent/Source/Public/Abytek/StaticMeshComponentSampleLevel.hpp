#pragma once

#include "Abytek/Sandbox.NFCSamples.StaticMeshComponent.prerequisites.pch.hpp"
#include "Abytek/Level/Level.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_STATIC_MESH_COMPONENT_API F_StaticMeshComponentSampleLevel final : public F_Level
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_Level)
        ABYTEK_END_REFLECTOR(F_StaticMeshComponentSampleLevel);
        
    public:
        F_StaticMeshComponentSampleLevel(const F_SerializableObjectInitParams& InitParams);
        ~F_StaticMeshComponentSampleLevel() override;
        
    protected:
        void OnLoadContent() override;
    };
}
