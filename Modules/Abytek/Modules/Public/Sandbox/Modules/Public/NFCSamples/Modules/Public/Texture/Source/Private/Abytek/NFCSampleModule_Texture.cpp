#include "Abytek/NFCSampleModule_Texture.hpp"
#include "Abytek/Assets/Texture.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_NFCSampleModule_Texture)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_NFCSampleModule_Texture"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_Texture)
    
    F_NFCSampleModule_Texture::F_NFCSampleModule_Texture(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_NFCSampleModule_Texture::~F_NFCSampleModule_Texture()
    {
    }

    void F_NFCSampleModule_Texture::OnReflect()
    {
    }

    void F_NFCSampleModule_Texture::OnStartup()
    {
        TS<F_Texture> Texture;
        if (
            H_Serializable::PopulateObject<F_Texture>(
                Texture,
                ABYTEK_NAME("DemoTexture"),
                ABYTEK_NAME("@Abytek.Sandbox.NFCSamples.Texture::Assets:/.IgnoreSVC/DemoTexture")
            )
        )
        {
            static constexpr E_RHIFormat Format = E_RHIFormat::R32_UINT;
            static constexpr U32 Width = 4;
            static constexpr U32 Height = 4;
            
            TF_RHIImageRaw<Format> ImageRaw(true, Width, Height);
            TF_RHIImageRawAccessor<true, Format> ImageRawAccessor(ImageRaw);
            for (U32 Y = 0; Y < Height; ++Y)
            {
                for (U32 X = 0; X < Width; ++X)
                {
                    ImageRawAccessor[F_Vector2_U32(X, Y)] = X + Y;
                }
            }
            Texture->Import(ImageRaw);
            Texture->GetPackage()->Save();
        }
        
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            []
            {
                H_RHI::SignalCapture();
            }
        );
    }
}
