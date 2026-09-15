#include "Abytek/ActorComponents/Render/CameraRenderProxy.hpp"
#include "Abytek/ActorComponents/Render/CanvasRenderProxy.hpp"
#include "Abytek/Assets/Render/TextureRenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/RenderBase/RenderScene.hpp"
#include "Abytek/RenderBase/RenderViewFamily.hpp"
#include "Abytek/RenderBase/RenderView.hpp"
#include "Abytek/RenderBase/Renderer.hpp"
#include "Abytek/RenderBase/RenderObjectFactory.hpp"
#include "Abytek/RenderBase/WorldRenderResource.hpp"


namespace Abytek
{
    F_CameraRenderProxy::F_CameraRenderProxy(const TW_Valid<F_CameraComponent>& Owner) :
        A_RenderableComponentProxy(Owner)
    {
    }
    F_CameraRenderProxy::~F_CameraRenderProxy()
    {
    }

    void F_CameraRenderProxy::OnInit_RenderTask()
    {
    }
    void F_CameraRenderProxy::OnRelease_RenderTask()
    {
    }

    void F_CameraRenderProxy::OnCreateRenderState_RenderTask()
    {
        auto Scene = GetWorldRenderResource()->GetScene();
        auto RenderObjectFactory = GetRenderObjectFactory();
        
        F_RenderViewFamilyBuildParams ViewFamilyBuildParams;
        ViewFamilyBuildParams.CanvasRenderProxy = _CanvasRenderProxy;
        ViewFamilyBuildParams.CameraRenderProxy = ABYTEK_WTHIS();
        ViewFamilyBuildParams.Scene = Scene.Weak();
        _ViewFamily = RenderObjectFactory->CreateViewFamily(ViewFamilyBuildParams);
        
        F_RendererBuildParams RendererBuildParams;
        RendererBuildParams.Scene = Scene.Weak();
        RendererBuildParams.ViewFamily = _ViewFamily;
        _Renderer = RenderObjectFactory->CreateRenderer(RendererBuildParams);
    }
    void F_CameraRenderProxy::OnDestroyRenderState_RenderTask()
    {
        _ProjectionOptions = {};
        _ViewMatrix_StereoRight = Identity<F_Matrix4x4_F32>();
        _ViewMatrix_StereoLeft = Identity<F_Matrix4x4_F32>();
        _ViewMatrix_Mono = Identity<F_Matrix4x4_F32>();
        _Renderer = {};
        _ViewFamily = {};
        _CanvasRenderProxy = {};
    }

    void F_CameraRenderProxy::OnDraw_RenderTask()
    {
        _ViewFamily->BeginFrame();
        _Renderer->Render();
        _ViewFamily->EndFrame();
    }

    void F_CameraRenderProxy::Draw_RenderTask()
    {
        OnDraw_RenderTask();
    }
}
