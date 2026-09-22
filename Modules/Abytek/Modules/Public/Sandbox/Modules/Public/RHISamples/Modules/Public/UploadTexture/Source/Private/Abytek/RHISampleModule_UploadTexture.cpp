#include "Abytek/RHISampleModule_UploadTexture.hpp"
#include "Abytek/Frame/FrameAllocator.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RHISampleModule_UploadTexture)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RHISampleModule_UploadTexture"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_RHISampleModule_UploadTexture)
    
    F_RHISampleModule_UploadTexture::F_RHISampleModule_UploadTexture(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_RHISampleModule_UploadTexture::~F_RHISampleModule_UploadTexture()
    {
    }

    void F_RHISampleModule_UploadTexture::OnReflect()
    {
    }

    void F_RHISampleModule_UploadTexture::OnStartup()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            []
            {
                auto RHIContext = H_RHI::GetMainContext();
                auto RHIProcess = H_RHI::GetMainSubmissionQueue();
                
                static constexpr E_RHIFormat Format = E_RHIFormat::R32_UINT;
                U32 Width = 3;
                U32 Height = 3;
                
                F_RHITextureBuildParams TextureBuildParams;
                TextureBuildParams.Context = RHIContext.Weak();
                TextureBuildParams.Format = Format;
                TextureBuildParams.TextureAspect.Width = Width;
                TextureBuildParams.TextureAspect.Height = Height;
                TextureBuildParams.TextureAspect.DimensionCount = 2;
                TextureBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeSRVCapabilities();
                auto Texture = RACreateAndBuildShared<A_RHIResource>(TextureBuildParams);
                
                auto SubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(RHIProcess.Weak());
                SubmissionList->SetDebugName(ABYTEK_DEBUG_NAME("Main"));
                
                TF_RHIImageRaw<Format, TF_FrameAllocator<U8, E_FrameParamType::RENDER>> ImageRaw(true, Width, Height);
                TF_RHIImageRawAccessor<true, Format> ImageRawAccessor(ImageRaw);
                for (U32 Y = 0; Y < Height; ++Y)
                {
                    for (U32 X = 0; X < Width; ++X)
                    {
                        ImageRawAccessor[F_Vector2_U32(X, Y)] = X + Y;
                    }
                }
                TF_RHIImage Image(ImageRaw);
                
                H_RHISubmissionUtilities::UploadTexture(
                    SubmissionList,
                    Image,
                    Texture,
                    ABYTEK_DEBUG_NAME("UploadTexture")
                );
                
                RHIProcess->AddSubmissionItem(SubmissionList);
                
                H_RHI::SignalCapture();
            }
        );
    }
}
