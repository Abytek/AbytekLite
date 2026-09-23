#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"
#include "Abytek/ActorComponents/Render/CanvasComponentRenderProxy.hpp"
#include "Abytek/ActorComponents/Render/CameraComponentRenderProxy.hpp"


namespace Abytek
{
    void A_RenderViewFamily::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderViewFamilyBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _CanvasComponentRenderProxy = BuildParams.CanvasComponentRenderProxy;
        _CameraComponentRenderProxy = BuildParams.CameraComponentRenderProxy;
        _Scene = BuildParams.Scene;
        switch (_CanvasComponentRenderProxy->GetTopology())
        {
        case E_CanvasTopology::MONO:
            {
                F_RenderViewBuildParams ViewBuildParams;
                ViewBuildParams.Family = ABYTEK_WTHIS();
                ViewBuildParams.Flags = (
                    E_RenderViewFlag::NONE
                    | E_RenderViewFlag::PRIMARY
                    | E_RenderViewFlag::MONO
                );
                ViewBuildParams.ProjectionOptions = _CameraComponentRenderProxy->GetProjectionOptions();
                ViewBuildParams.Resolution = _CanvasComponentRenderProxy->GetResolution_Mono();
                ViewBuildParams.AspectRatio = _CanvasComponentRenderProxy->GetAspectRatio_Mono();
                ViewBuildParams.ViewMatrix = _CameraComponentRenderProxy->GetViewMatrix_Mono();
                ViewBuildParams.RTV = _CanvasComponentRenderProxy->GetRTV_RTTexture_Mono();
                auto View = GetRenderObjectFactory()->CreateView();
#ifdef ABYTEK_DEBUG_INFO
                View->SetDebugName(
                    *GetDebugName()
                    + ABYTEK_TEXT(".ViewMono")
                );
#endif
                View->Init(SubmissionItemContainer, ViewBuildParams);
                AddView(View);
            }
            break;
        case E_CanvasTopology::STEREO:
            {
                F_RenderViewBuildParams ViewLeftBuildParams;
                ViewLeftBuildParams.Family = ABYTEK_WTHIS();
                ViewLeftBuildParams.Flags = (
                    E_RenderViewFlag::NONE
                    | E_RenderViewFlag::PRIMARY
                    | E_RenderViewFlag::STEREO_LEFT
                );
                ViewLeftBuildParams.ProjectionOptions = _CameraComponentRenderProxy->GetProjectionOptions();
                ViewLeftBuildParams.Resolution = _CanvasComponentRenderProxy->GetResolution_StereoLeft();
                ViewLeftBuildParams.AspectRatio = _CanvasComponentRenderProxy->GetAspectRatio_StereoLeft();
                ViewLeftBuildParams.ViewMatrix = _CameraComponentRenderProxy->GetViewMatrix_StereoLeft();
                ViewLeftBuildParams.RTV = _CanvasComponentRenderProxy->GetRTV_RTTexture_StereoLeft();
                auto ViewLeft = GetRenderObjectFactory()->CreateView();
                ViewLeft->Init(SubmissionItemContainer, ViewLeftBuildParams);
#ifdef ABYTEK_DEBUG_INFO
                ViewLeft->SetDebugName(
                    *GetDebugName()
                    + ABYTEK_TEXT(".ViewLeft")
                );
#endif
                AddView(ViewLeft);
                
                F_RenderViewBuildParams ViewRightBuildParams;
                ViewRightBuildParams.Family = ABYTEK_WTHIS();
                ViewRightBuildParams.Flags = (
                    E_RenderViewFlag::NONE
                    | E_RenderViewFlag::PRIMARY
                    | E_RenderViewFlag::STEREO_RIGHT
                );
                ViewRightBuildParams.ProjectionOptions = _CameraComponentRenderProxy->GetProjectionOptions();
                ViewRightBuildParams.Resolution = _CanvasComponentRenderProxy->GetResolution_StereoRight();
                ViewRightBuildParams.AspectRatio = _CanvasComponentRenderProxy->GetAspectRatio_StereoRight();
                ViewRightBuildParams.ViewMatrix = _CameraComponentRenderProxy->GetViewMatrix_StereoRight();
                ViewRightBuildParams.RTV = _CanvasComponentRenderProxy->GetRTV_RTTexture_StereoRight();
                auto ViewRight = GetRenderObjectFactory()->CreateView();
                ViewRight->Init(SubmissionItemContainer, ViewRightBuildParams);
#ifdef ABYTEK_DEBUG_INFO
                ViewRight->SetDebugName(
                    *GetDebugName()
                    + ABYTEK_TEXT(".ViewRight")
                );
#endif
                AddView(ViewRight);
            }
            break;
        default:
            ABYTEK_LOG_FATAL() << "Invalid canvas topology: " << static_cast<U32>(_CanvasComponentRenderProxy->GetTopology());
            break;
        }
    }
    void A_RenderViewFamily::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        for (const auto& View : _Views)
        {
            View->Release(SubmissionItemContainer);
        }
        _Views = {};
        
        _Scene = {};
        _CameraComponentRenderProxy = {};
        _CanvasComponentRenderProxy = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void A_RenderViewFamily::AddView(const TS<A_RenderView>& View)
    {
        _Views.push_back(View);
    }

    void A_RenderViewFamily::OnBeginFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        for (const auto& View : _Views)
        {
            if (View->HasFlags(E_RenderViewFlag::MONO))
            {
                View->SetResolution(_CanvasComponentRenderProxy->GetResolution_Mono());
                View->SetAspectRatio(_CanvasComponentRenderProxy->GetAspectRatio_Mono());
                View->SetViewMatrix(_CameraComponentRenderProxy->GetViewMatrix_Mono());
                View->SetRTV(_CanvasComponentRenderProxy->GetRTV_RTTexture_Mono());
            }
            if (View->HasFlags(E_RenderViewFlag::STEREO_LEFT))
            {
                View->SetResolution(_CanvasComponentRenderProxy->GetResolution_StereoLeft());
                View->SetAspectRatio(_CanvasComponentRenderProxy->GetAspectRatio_StereoLeft());
                View->SetViewMatrix(_CameraComponentRenderProxy->GetViewMatrix_StereoLeft());
                View->SetRTV(_CanvasComponentRenderProxy->GetRTV_RTTexture_StereoLeft());
            }
            if (View->HasFlags(E_RenderViewFlag::STEREO_RIGHT))
            {
                View->SetResolution(_CanvasComponentRenderProxy->GetResolution_StereoRight());
                View->SetAspectRatio(_CanvasComponentRenderProxy->GetAspectRatio_StereoRight());
                View->SetViewMatrix(_CameraComponentRenderProxy->GetViewMatrix_StereoRight());
                View->SetRTV(_CanvasComponentRenderProxy->GetRTV_RTTexture_StereoRight());
            }
            View->BeginFrame(SubmissionItemContainer);
        }
    }
    void A_RenderViewFamily::OnEndFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        for (const auto& View : _Views)
        {
            View->EndFrame(SubmissionItemContainer);
        }
    }

    void A_RenderViewFamily::BeginFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
#ifdef ABYTEK_DEBUG_INFO
        SubmissionItemContainer->PushAdditionalStackCaptureEventStates(
            ABYTEK_TEXT("Abytek::A_RenderViewFamily::Frame(") + *GetDebugName() + ABYTEK_TEXT(")")
        );
#endif
        OnBeginFrame(SubmissionItemContainer);
    }
    void A_RenderViewFamily::EndFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        OnEndFrame(SubmissionItemContainer);
#ifdef ABYTEK_DEBUG_INFO
        SubmissionItemContainer->PopAdditionalStackCaptureEventStates();
#endif
    }
}
