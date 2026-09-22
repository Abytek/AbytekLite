#include "Abytek/Assets/Render/TextureRenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    F_TextureRenderProxy::F_TextureRenderProxy(const TW_Valid<F_Texture>& Owner) :
        A_WorldContextRenderProxy(Owner)
    {
    }
    F_TextureRenderProxy::~F_TextureRenderProxy()
    {
    }

    void F_TextureRenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        F_RHITextureBuildParams RHITextureBuildParams;
        RHITextureBuildParams.Context = H_RHI::GetMainContext().Weak();
        RHITextureBuildParams.Format = _TempImage->Setting.Format;
        static_cast<F_RHIImageSettingMinimal&>(RHITextureBuildParams.TextureAspect) = static_cast<const F_RHIImageSettingMinimal&>(
            _TempImage->Setting
        );
        if (_TempImage->Payload.Enable)
        {
            RHITextureBuildParams.TextureDataView = *_TempImage;
        }
        RHITextureBuildParams.AccessCapabilities = _Setting.GetRHIResourceAccessCapabilities();
        RHITextureBuildParams.AdditionalFlags = _Setting.GetRHIResourceAdditionalFlags();
        RHITextureBuildParams.StaticAccess = _Setting.GetRHIResourceStaticAccess();
        _RHITexture = RACreateAndBuildShared<A_RHIResource>(RHITextureBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        _RHITexture->SetDebugName(GetDebugName());
#endif
        
        H_Frame::EnqueueCommand<E_FrameParamType::DISPLAY, E_FrameParamType::RENDER>(
            [TempImage = ABYTEK_MOVE(_TempImage)]
            {}    
        );
    }
    void F_TextureRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _ImageSettingMinimal = {};
        _Format = E_RHIFormat::NONE;
        _RHITexture = {};
    }
}
