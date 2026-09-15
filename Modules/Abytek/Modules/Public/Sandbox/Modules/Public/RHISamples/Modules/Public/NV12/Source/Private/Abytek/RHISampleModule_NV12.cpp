#include "Abytek/RHISampleModule_NV12.hpp"
#include "Abytek/Frame/FrameAllocator.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RHISampleModule_NV12)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RHISampleModule_NV12"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_RHISampleModule_NV12)
    
    F_RHISampleModule_NV12::F_RHISampleModule_NV12(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_RHISampleModule_NV12::~F_RHISampleModule_NV12()
    {
    }

    void F_RHISampleModule_NV12::OnReflect()
    {
    }

    void F_RHISampleModule_NV12::OnStartup()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            []
            {
                auto RHIContext = H_RHI::GetMainContext();
                auto RHIProcess = H_RHI::GetMainProcess();
                
                static constexpr E_RHIFormat Format = E_RHIFormat::NV12;
                U32 Width = 4;
                U32 Height = 4;
                
                F_RHITextureBuildParams TextureBuildParams;
                TextureBuildParams.Context = RHIContext.Weak();
                TextureBuildParams.Format = Format;
                TextureBuildParams.TextureAspect.Width = Width;
                TextureBuildParams.TextureAspect.Height = Height;
                TextureBuildParams.TextureAspect.DimensionCount = 2;
                TextureBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeSRVCapabilities();
                auto Texture = RACreateAndBuildShared<A_RHIResource>(TextureBuildParams);
                
                H_RHI::SignalCapture();
            }
        );
    }
}
