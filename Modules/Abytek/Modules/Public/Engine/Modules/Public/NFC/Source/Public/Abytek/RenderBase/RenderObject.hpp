#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"


namespace Abytek
{
    class F_WorldRenderResource;
    class A_RenderObjectFactory;
    class F_RenderRegistryRuntime;
    struct F_RenderCoreRHIConfig;
    
    class ABYTEK_ENGINE_NFC_API A_RenderObject : public A_Object
    {
    private:
        TW<F_WorldRenderResource> _WorldRenderResource;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetWorldRenderResource() const noexcept
        {
            return _WorldRenderResource;
        }
        
    protected:
        A_RenderObject(const TW_Valid<F_WorldRenderResource>& WorldRenderResource);
        
    public:
        ~A_RenderObject() override;
        
    public:
        const TS<A_RenderObjectFactory>& GetRenderObjectFactory() const noexcept;
        const TS<F_RenderRegistryRuntime>& GetRenderRegistryRuntime() const noexcept;
        
    public:
        const F_RenderCoreRHIConfig& GetRHIConfig() const noexcept;
        const F_RHIFeatureSupports& GetRHIFeatureSupports() const noexcept;
    };
}
