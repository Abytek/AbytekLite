#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"
#include "Abytek/ActorComponents/Render/CanvasRenderProxy.hpp"
#include "Abytek/ActorComponents/Render/CameraRenderProxy.hpp"


namespace Abytek
{
    void A_RenderViewFamily::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderViewFamilyBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _CanvasRenderProxy = BuildParams.CanvasRenderProxy;
        _CameraRenderProxy = BuildParams.CameraRenderProxy;
        _Scene = BuildParams.Scene;
        switch (_CanvasRenderProxy->GetTopology())
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
                ViewBuildParams.ProjectionOptions = _CameraRenderProxy->GetProjectionOptions();
                ViewBuildParams.Resolution = _CanvasRenderProxy->GetResolution_Mono();
                ViewBuildParams.AspectRatio = _CanvasRenderProxy->GetAspectRatio_Mono();
                ViewBuildParams.ViewMatrix = _CameraRenderProxy->GetViewMatrix_Mono();
                ViewBuildParams.RTV = _CanvasRenderProxy->GetRTV_RTTexture_Mono();
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
                ViewLeftBuildParams.ProjectionOptions = _CameraRenderProxy->GetProjectionOptions();
                ViewLeftBuildParams.Resolution = _CanvasRenderProxy->GetResolution_StereoLeft();
                ViewLeftBuildParams.AspectRatio = _CanvasRenderProxy->GetAspectRatio_StereoLeft();
                ViewLeftBuildParams.ViewMatrix = _CameraRenderProxy->GetViewMatrix_StereoLeft();
                ViewLeftBuildParams.RTV = _CanvasRenderProxy->GetRTV_RTTexture_StereoLeft();
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
                ViewRightBuildParams.ProjectionOptions = _CameraRenderProxy->GetProjectionOptions();
                ViewRightBuildParams.Resolution = _CanvasRenderProxy->GetResolution_StereoRight();
                ViewRightBuildParams.AspectRatio = _CanvasRenderProxy->GetAspectRatio_StereoRight();
                ViewRightBuildParams.ViewMatrix = _CameraRenderProxy->GetViewMatrix_StereoRight();
                ViewRightBuildParams.RTV = _CanvasRenderProxy->GetRTV_RTTexture_StereoRight();
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
            ABYTEK_LOG_FATAL() << "Invalid canvas topology: " << static_cast<U32>(_CanvasRenderProxy->GetTopology());
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
        _CameraRenderProxy = {};
        _CanvasRenderProxy = {};
        
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
                View->SetResolution(_CanvasRenderProxy->GetResolution_Mono());
                View->SetAspectRatio(_CanvasRenderProxy->GetAspectRatio_Mono());
                View->SetViewMatrix(_CameraRenderProxy->GetViewMatrix_Mono());
                View->SetRTV(_CanvasRenderProxy->GetRTV_RTTexture_Mono());
            }
            if (View->HasFlags(E_RenderViewFlag::STEREO_LEFT))
            {
                View->SetResolution(_CanvasRenderProxy->GetResolution_StereoLeft());
                View->SetAspectRatio(_CanvasRenderProxy->GetAspectRatio_StereoLeft());
                View->SetViewMatrix(_CameraRenderProxy->GetViewMatrix_StereoLeft());
                View->SetRTV(_CanvasRenderProxy->GetRTV_RTTexture_StereoLeft());
            }
            if (View->HasFlags(E_RenderViewFlag::STEREO_RIGHT))
            {
                View->SetResolution(_CanvasRenderProxy->GetResolution_StereoRight());
                View->SetAspectRatio(_CanvasRenderProxy->GetAspectRatio_StereoRight());
                View->SetViewMatrix(_CameraRenderProxy->GetViewMatrix_StereoRight());
                View->SetRTV(_CanvasRenderProxy->GetRTV_RTTexture_StereoRight());
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
