#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxy_StaticMeshECMS.hpp"


namespace Abytek
{
    A_GeometryFactoryTypeProxy_StaticMeshECMS::A_GeometryFactoryTypeProxy_StaticMeshECMS(const TW_Valid<A_GeometryFactoryType>& GeometryFactoryType) :
        A_GeometryFactoryTypeProxy(GeometryFactoryType)
    {
    }
    A_GeometryFactoryTypeProxy_StaticMeshECMS::~A_GeometryFactoryTypeProxy_StaticMeshECMS()
    {
    }

    void A_GeometryFactoryTypeProxy_StaticMeshECMS::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_GeometryFactoryTypeProxy::OnCreateRenderState_RenderTask(SubmissionItemContainer);
    }
    void A_GeometryFactoryTypeProxy_StaticMeshECMS::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_GeometryFactoryTypeProxy::OnDestroyRenderState_RenderTask(SubmissionItemContainer);
    }
}
