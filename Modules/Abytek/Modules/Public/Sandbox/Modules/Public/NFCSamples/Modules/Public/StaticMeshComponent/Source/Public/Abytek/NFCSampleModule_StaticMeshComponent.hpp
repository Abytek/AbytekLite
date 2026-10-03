#pragma once

#include "Abytek/Sandbox.NFCSamples.StaticMeshComponent.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_STATIC_MESH_COMPONENT_API F_NFCSampleModule_StaticMeshComponent final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_NFCSampleModule_StaticMeshComponent);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_StaticMeshComponent);
        
    public:
        F_NFCSampleModule_StaticMeshComponent(const F_ProgramUnitBuildParams& BuildParams);
        ~F_NFCSampleModule_StaticMeshComponent() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
