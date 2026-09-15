#include "Abytek/NFCSampleModule_TextureImportFile.hpp"
#include "Abytek/Assets/Texture.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_NFCSampleModule_TextureImportFile)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_NFCSampleModule_TextureImportFile"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_TextureImportFile)
    
    F_NFCSampleModule_TextureImportFile::F_NFCSampleModule_TextureImportFile(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_NFCSampleModule_TextureImportFile::~F_NFCSampleModule_TextureImportFile()
    {
    }

    void F_NFCSampleModule_TextureImportFile::OnReflect()
    {
    }

    void F_NFCSampleModule_TextureImportFile::OnStartup()
    {
        {
            TS<F_Texture> Texture;
            if (
                H_Serializable::PopulateObject<F_Texture>(
                    Texture,
                    ABYTEK_NAME("AbytekIcon"),
                    ABYTEK_NAME("@Abytek.Sandbox.NFCSamples.TextureImportFile::Assets:/.IgnoreSVC/AbytekIcon")
                )
            )
            {
                Texture->Import(ABYTEK_TEXT("@Abytek.Sandbox.NFCSamples.TextureImportFile::Assets:/AbytekIcon.png"));
                Texture->GetPackage()->Save();
            }
        }
        {
            TS<F_Texture> Texture;
            if (
                H_Serializable::PopulateObject<F_Texture>(
                    Texture,
                    ABYTEK_NAME("cobblestone_parish_road_4k"),
                    ABYTEK_NAME("@Abytek.Sandbox.NFCSamples.TextureImportFile::Assets:/.IgnoreSVC/cobblestone_parish_road_4k")
                )
            )
            {
                Texture->Import(ABYTEK_TEXT("@Abytek.Sandbox.NFCSamples.TextureImportFile::Assets:/cobblestone_parish_road_4k.hdr"));
                Texture->GetPackage()->Save();
            }
        }
        
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            []
            {
                H_RHI::SignalCapture();
            }
        );
    }
}
