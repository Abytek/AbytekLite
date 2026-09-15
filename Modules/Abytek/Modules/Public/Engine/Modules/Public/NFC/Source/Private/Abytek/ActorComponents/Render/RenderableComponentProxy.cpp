#include "Abytek/ActorComponents/Render/RenderableComponentProxy.hpp"
#include "Abytek/ActorComponents/RenderableComponent.hpp"


namespace Abytek
{
    A_RenderableComponentProxy::A_RenderableComponentProxy(const TW_Valid<A_RenderableComponent>& Component) :
        A_WorldContextRenderProxy(Component)
    {
    }
    A_RenderableComponentProxy::~A_RenderableComponentProxy()
    {
    }
}
