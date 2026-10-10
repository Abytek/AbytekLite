#include "Abytek/Renderer/StandardPrimitive/SRPGeometryFactoryTypeProxy_StaticMeshECMS.hpp"


namespace Abytek
{
    F_SRPGeometryFactoryTypeProxy_StaticMeshECMS::F_SRPGeometryFactoryTypeProxy_StaticMeshECMS(const TW_Valid<A_GeometryFactoryType>& GeometryFactoryType) :
        A_GeometryFactoryTypeProxy_StaticMeshECMS(GeometryFactoryType)
    {
    }
    F_SRPGeometryFactoryTypeProxy_StaticMeshECMS::~F_SRPGeometryFactoryTypeProxy_StaticMeshECMS()
    {
    }

    void F_SRPGeometryFactoryTypeProxy_StaticMeshECMS::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_GeometryFactoryTypeProxy_StaticMeshECMS::OnCreateRenderState_RenderTask(SubmissionItemContainer);
    }
    void F_SRPGeometryFactoryTypeProxy_StaticMeshECMS::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_GeometryFactoryTypeProxy_StaticMeshECMS::OnDestroyRenderState_RenderTask(SubmissionItemContainer);
    }
}
