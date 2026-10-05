#include "Abytek/Renderer/WorldRenderResourceChild.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderable.hpp"
#include "Abytek/World/WorldContext.hpp"


namespace Abytek
{
    A_WorldRenderResourceChild::A_WorldRenderResourceChild(const TW_Valid<A_WorldContext>& WorldContext) :
        A_RenderProxy(WorldContext.DynamicCast<A_Renderable>()),
        _WorldRenderResource(F_WorldRenderResource::Get_MainTask(WorldContext).Weak())
    {
    }
    A_WorldRenderResourceChild::~A_WorldRenderResourceChild()
    {
    }

    const TS<A_RenderObjectFactory>& A_WorldRenderResourceChild::GetRenderObjectFactory() const noexcept
    {
        return _WorldRenderResource->GetRenderObjectFactory();
    }
}
