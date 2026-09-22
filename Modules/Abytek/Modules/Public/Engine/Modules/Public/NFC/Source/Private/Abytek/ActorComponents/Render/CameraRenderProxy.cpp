#include "Abytek/ActorComponents/Render/CameraRenderProxy.hpp"
#include "Abytek/ActorComponents/Render/CanvasRenderProxy.hpp"
#include "Abytek/Assets/Render/TextureRenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/Renderer/Renderer.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"


namespace Abytek
{
    F_CameraRenderProxy::F_CameraRenderProxy(const TW_Valid<F_CameraComponent>& Owner) :
        A_RenderableComponentProxy(Owner)
    {
    }
    F_CameraRenderProxy::~F_CameraRenderProxy()
    {
    }

    void F_CameraRenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto Scene = GetWorldRenderResource()->GetScene();
        auto RenderObjectFactory = GetRenderObjectFactory();
        
        F_RenderViewFamilyBuildParams ViewFamilyBuildParams;
        ViewFamilyBuildParams.CanvasRenderProxy = _CanvasRenderProxy;
        ViewFamilyBuildParams.CameraRenderProxy = ABYTEK_WTHIS();
        ViewFamilyBuildParams.Scene = Scene.Weak();
        _ViewFamily = RenderObjectFactory->CreateViewFamily();
#ifdef ABYTEK_DEBUG_INFO
        _ViewFamily->SetDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".ViewFamily")
        );
#endif
        _ViewFamily->Init(SubmissionItemContainer, ViewFamilyBuildParams);
        
        F_RendererBuildParams RendererBuildParams;
        RendererBuildParams.Scene = Scene.Weak();
        RendererBuildParams.ViewFamily = _ViewFamily;
        _Renderer = RenderObjectFactory->CreateRenderer();
#ifdef ABYTEK_DEBUG_INFO
        _Renderer->SetDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".Renderer")
        );
#endif
        _Renderer->Init(SubmissionItemContainer, RendererBuildParams);
    }
    void F_CameraRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _ProjectionOptions = {};
        _ViewMatrix_StereoRight = Identity<F_Matrix4x4_F32>();
        _ViewMatrix_StereoLeft = Identity<F_Matrix4x4_F32>();
        _ViewMatrix_Mono = Identity<F_Matrix4x4_F32>();
        _Renderer = {};
        _ViewFamily = {};
        _CanvasRenderProxy = {};
    }

    void F_CameraRenderProxy::OnDraw_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _ViewFamily->BeginFrame(SubmissionItemContainer);
        _Renderer->Render(SubmissionItemContainer);
        _ViewFamily->EndFrame(SubmissionItemContainer);
    }

    void F_CameraRenderProxy::Draw_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        ABYTEK_RHI_CAPTURE_EVENT_SCOPE(
            SubmissionItemContainer,  
            ABYTEK_TEXT("Abytek::F_CameraRenderProxy::Draw(") + *GetDebugName() + ABYTEK_TEXT(")")
        );
        OnDraw_RenderTask(SubmissionItemContainer);
    }
}
