#include "Abytek/ActorComponents/Render/PrimitiveComponentRenderProxy.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/Renderer/Renderer.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"


namespace Abytek
{
    A_PrimitiveComponentRenderProxy::A_PrimitiveComponentRenderProxy(const TW_Valid<A_PrimitiveComponent>& Owner) :
        A_RenderableComponentProxy(Owner)
    {
    }
    A_PrimitiveComponentRenderProxy::~A_PrimitiveComponentRenderProxy()
    {
    }

    void A_PrimitiveComponentRenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _PrimitiveSet = CreatePrimitiveSet(SubmissionItemContainer);
    }
    void A_PrimitiveComponentRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _PrimitiveSet = {};
    }

    TS<F_RenderPrimitiveSet> A_PrimitiveComponentRenderProxy::CreatePrimitiveSet(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto PrimitiveManager = GetWorldRenderResource()->GetScene()->GetPrimitiveManager();
        F_RenderPrimitiveSetBuildParams BuildParams;
        BuildParams.Manager = PrimitiveManager.Weak();
        BuildParams.Num = 1;
        SetupPrimitiveSetBuildParams(BuildParams);
#ifdef ABYTEK_DEBUG_INFO
        auto PrimitiveSet = F_RenderPrimitiveSet::CreateAndInit_WithDebugName(
            GetDebugName(),
#else
        auto PrimitiveSet = F_RenderPrimitiveSet::CreateAndInit(
#endif
            GetWorldRenderResource(),
            SubmissionItemContainer,
            BuildParams
        );
        return PrimitiveSet;
    }
    void A_PrimitiveComponentRenderProxy::SetupPrimitiveSetBuildParams(F_RenderPrimitiveSetBuildParams& OutBuildParams)
    {
    }
}
