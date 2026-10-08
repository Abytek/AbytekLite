#include "Abytek/Renderer/VertexFactory/VertexFactoryTypeProxy.hpp"
#include "Abytek/Renderer/VertexFactory/VertexFactoryTypeManager.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderer/RenderScene.hpp"


namespace Abytek
{
    A_VertexFactoryTypeProxy::A_VertexFactoryTypeProxy(const TW_Valid<A_VertexFactoryType>& VertexFactoryType) :
        A_WorldRenderResourceChild(VertexFactoryType)
    {
    }
    A_VertexFactoryTypeProxy::~A_VertexFactoryTypeProxy()
    {
    }

    void A_VertexFactoryTypeProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto Manager = GetWorldRenderResource()->GetScene()->GetVertexFactoryTypeManager();
        Manager->_RegisterType(ABYTEK_WTHIS());
    }
    void A_VertexFactoryTypeProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto Manager = GetWorldRenderResource()->GetScene()->GetVertexFactoryTypeManager();
        Manager->_RegisterType(ABYTEK_WTHIS());
    }
}
