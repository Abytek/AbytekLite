#pragma once

#include "Abytek/Sandbox.NFCSamples.TextureImportFile.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_TEXTURE_IMPORT_FILE_API F_NFCSampleModule_TextureImportFile final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_NFCSampleModule_TextureImportFile)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_TextureImportFile);
        
    public:
        F_NFCSampleModule_TextureImportFile(const F_ProgramUnitBuildParams& BuildParams);
        ~F_NFCSampleModule_TextureImportFile() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
