#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/RenderProxy.hpp"


namespace Abytek
{
    class A_WorldContext;
    class F_WorldRenderResource;
    class A_RenderObjectFactory;
    class F_RenderRegistryRuntime;

    class ABYTEK_ENGINE_NFC_API A_WorldContextRenderProxy : public A_RenderProxy
    {
    private:
        TW<F_WorldRenderResource> _WorldRenderResource;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetWorldRenderResource() const
        {
            return _WorldRenderResource;
        }
        
    protected:
        A_WorldContextRenderProxy(const TW_Valid<A_WorldContext>& Component);
        
    public:
        ~A_WorldContextRenderProxy() override;
        
    public:
        const TS<A_RenderObjectFactory>& GetRenderObjectFactory() const noexcept;
    };
}