#include "Abytek/ActorComponents/Render/StaticMeshComponentRenderProxy.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/Renderer/Renderer.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"


namespace Abytek
{
    F_StaticMeshComponentRenderProxy::F_StaticMeshComponentRenderProxy(const TW_Valid<F_StaticMeshComponent>& Owner) :
        A_PrimitiveComponentRenderProxy(Owner)
    {
    }
    F_StaticMeshComponentRenderProxy::~F_StaticMeshComponentRenderProxy()
    {
    }

    void F_StaticMeshComponentRenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_PrimitiveComponentRenderProxy::OnCreateRenderState_RenderTask(SubmissionItemContainer);
    }
    void F_StaticMeshComponentRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_PrimitiveComponentRenderProxy::OnDestroyRenderState_RenderTask(SubmissionItemContainer);
    }
}
