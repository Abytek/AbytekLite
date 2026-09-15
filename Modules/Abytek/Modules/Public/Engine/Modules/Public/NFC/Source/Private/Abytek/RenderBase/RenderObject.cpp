#include "Abytek/RenderBase/RenderObject.hpp"
#include "Abytek/RenderBase/WorldRenderResource.hpp"


namespace Abytek
{
    A_RenderObject::A_RenderObject(const TW_Valid<F_WorldRenderResource>& WorldRenderResource) :
        _WorldRenderResource(WorldRenderResource)
    {
    }
    A_RenderObject::~A_RenderObject()
    {
    }

    const TS<A_RenderObjectFactory>& A_RenderObject::GetRenderObjectFactory() const noexcept
    {
        return _WorldRenderResource->GetRenderObjectFactory();
    }
    const TS<F_RenderRegistryRuntime>& A_RenderObject::GetRenderRegistryRuntime() const noexcept
    {
        return _WorldRenderResource->GetRenderRegistryRuntime();
    }

    const F_RenderCoreRHIConfig& A_RenderObject::GetRHIConfig() const noexcept
    {
        return _WorldRenderResource->GetRHIConfig();
    }
    const F_RHIFeatureSupports& A_RenderObject::GetRHIFeatureSupports() const noexcept
    {
        return _WorldRenderResource->GetRHIFeatureSupports();
    }
}
