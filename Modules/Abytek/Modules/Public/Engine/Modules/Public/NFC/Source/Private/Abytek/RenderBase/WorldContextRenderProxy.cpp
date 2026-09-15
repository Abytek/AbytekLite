#include "Abytek/RenderBase/WorldContextRenderProxy.hpp"
#include "Abytek/RenderBase/WorldRenderResource.hpp"
#include "Abytek/Renderable.hpp"
#include "Abytek/World/WorldContext.hpp"


namespace Abytek
{
    A_WorldContextRenderProxy::A_WorldContextRenderProxy(const TW_Valid<A_WorldContext>& WorldContext) :
        A_RenderProxy(WorldContext.DynamicCast<A_Renderable>()),
        _WorldRenderResource(F_WorldRenderResource::Get_MainTask(WorldContext).Weak())
    {
    }
    A_WorldContextRenderProxy::~A_WorldContextRenderProxy()
    {
    }

    const TS<A_RenderObjectFactory>& A_WorldContextRenderProxy::GetRenderObjectFactory() const noexcept
    {
        return _WorldRenderResource->GetRenderObjectFactory();
    }
}
