#include "Abytek/RenderBase/RenderViewFamily.hpp"
#include "Abytek/RenderBase/RenderObjectFactory.hpp"
#include "Abytek/ActorComponents/Render/CanvasRenderProxy.hpp"
#include "Abytek/ActorComponents/Render/CameraRenderProxy.hpp"


namespace Abytek
{
    A_RenderViewFamily::A_RenderViewFamily(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderViewFamilyBuildParams& BuildParams) :
        A_RenderObject(WorldRenderResource),
        _CanvasRenderProxy(BuildParams.CanvasRenderProxy),
        _CameraRenderProxy(BuildParams.CameraRenderProxy),
        _Scene(BuildParams.Scene)
    {
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
                auto View = GetRenderObjectFactory()->CreateView(ViewBuildParams);
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
                auto ViewLeft = GetRenderObjectFactory()->CreateView(ViewLeftBuildParams);
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
                auto ViewRight = GetRenderObjectFactory()->CreateView(ViewRightBuildParams);
                AddView(ViewRight);
            }
            break;
        default:
            ABYTEK_LOG_FATAL() << "Invalid canvas topology: " << static_cast<U32>(_CanvasRenderProxy->GetTopology());
            break;
        }
    }
    A_RenderViewFamily::~A_RenderViewFamily()
    {
    }

    void A_RenderViewFamily::AddView(const TS<A_RenderView>& View)
    {
        _Views.push_back(View);
    }

    void A_RenderViewFamily::OnBeginFrame()
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
            View->BeginFrame();
        }
    }
    void A_RenderViewFamily::OnEndFrame()
    {
        for (const auto& View : _Views)
        {
            View->EndFrame();
        }
    }

    void A_RenderViewFamily::BeginFrame()
    {
        OnBeginFrame();
    }
    void A_RenderViewFamily::EndFrame()
    {
        OnEndFrame();
    }
}
