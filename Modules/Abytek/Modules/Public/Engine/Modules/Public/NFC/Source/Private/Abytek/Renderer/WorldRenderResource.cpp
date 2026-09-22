#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderer/RendererManager.hpp"
#include "Abytek/Renderer/RenderPath.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/World/WorldContextHelper.hpp"


namespace Abytek
{
    TS<F_WorldRenderResource> F_WorldRenderResource::Get_MainTask(const TW_Valid<A_WorldContext>& WorldContext)
    {
        return H_WorldContext::GetUnit<F_RendererManager>(WorldContext)->GetRenderProxy().FastCast<F_WorldRenderResource>();
    }

    F_WorldRenderResource::F_WorldRenderResource(const TW_Valid<F_RendererManager>& RendererManager) :
        A_RenderProxy(RendererManager)
    {
#ifdef ABYTEK_DEBUG_INFO
        _WorldDebugName = RendererManager->GetWorld()->GetDebugName();
#endif
        auto RenderPath = RendererManager->GetRenderPath();
        _RenderObjectFactory = RenderPath->CreateObjectFactory(ABYTEK_WTHIS());
    }
    F_WorldRenderResource::~F_WorldRenderResource()
    {
        _RenderObjectFactory = {};
#ifdef ABYTEK_DEBUG_INFO
        _WorldDebugName = {};
#endif
    }

    void F_WorldRenderResource::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        F_RenderSceneBuildParams SceneBuildParams;
        _Scene = _RenderObjectFactory->CreateScene();
#ifdef ABYTEK_DEBUG_INFO
        _Scene->SetDebugName(
            *_WorldDebugName
            + ABYTEK_TEXT(".Scene")  
        );
#endif
        _Scene->Init(SubmissionItemContainer, SceneBuildParams);
    }
    void F_WorldRenderResource::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _Scene->Release(SubmissionItemContainer);
        _Scene = {};
    }
}
