#pragma once

#include "Abytek/Sandbox.NFCSamples.StaticMeshImportFile.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_STATIC_MESH_IMPORT_FILE_API F_NFCSampleModule_StaticMeshImportFile final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_NFCSampleModule_StaticMeshImportFile)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_StaticMeshImportFile);
        
    public:
        F_NFCSampleModule_StaticMeshImportFile(const F_ProgramUnitBuildParams& BuildParams);
        ~F_NFCSampleModule_StaticMeshImportFile() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
