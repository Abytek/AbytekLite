#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxy.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxyManager.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderer/RenderScene.hpp"


namespace Abytek
{
    A_GeometryFactoryTypeProxy::A_GeometryFactoryTypeProxy(const TW_Valid<A_GeometryFactoryType>& GeometryFactoryType) :
        A_WorldRenderResourceChild(GeometryFactoryType)
    {
    }
    A_GeometryFactoryTypeProxy::~A_GeometryFactoryTypeProxy()
    {
    }

    void A_GeometryFactoryTypeProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto Manager = GetWorldRenderResource()->GetScene()->GetGeometryFactoryTypeProxyManager();
        Manager->_RegisterTypeProxy(ABYTEK_WTHIS());
    }
    void A_GeometryFactoryTypeProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto Manager = GetWorldRenderResource()->GetScene()->GetGeometryFactoryTypeProxyManager();
        Manager->_RegisterTypeProxy(ABYTEK_WTHIS());
        
        _HashCode = 0;
    }
}
