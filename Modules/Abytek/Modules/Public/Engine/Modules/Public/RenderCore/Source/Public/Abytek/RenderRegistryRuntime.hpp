#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"


namespace Abytek
{
    class F_RenderRegistry;
    class F_RenderRegistryRuntime;
    
    struct F_RenderRegistryRuntimeBuildParams
    {
        TS<A_RHIContext> Context;
        F_RHIFeatureSupports RHIFeatureSupports;
        TF_Vector<TS<F_RenderRegistryRuntime>> Dependencies;
    };
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderRegistryRuntime final : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_RenderRegistryRuntime);
        
    private:
        TS<A_RHIContext> _Context;
        F_RHIFeatureSupports _RHIFeatureSupports;
        TF_Vector<TS<F_RenderRegistryRuntime>> _Dependencies;
        TS<A_RHITemplateDatabase> _TemplateDatabase;
        TW<A_RHITemplateRuntimeDatabase> _TemplateRuntimeDatabase;

    public:
        ABYTEK_FORCE_INLINE const auto& GetContext() const noexcept
        {
            return _Context;
        }
        ABYTEK_FORCE_INLINE const auto& GetRHIFeatureSupports() const noexcept
        {
            return _RHIFeatureSupports;
        }
        ABYTEK_FORCE_INLINE const auto& GetDependencies() const noexcept
        {
            return _Dependencies;
        }
        ABYTEK_FORCE_INLINE const auto& GetTemplateDatabase() const noexcept
        {
            return _TemplateDatabase;
        }
        ABYTEK_FORCE_INLINE const auto& GetTemplateRuntimeDatabase() const noexcept
        {
            return _TemplateRuntimeDatabase;
        }
        
    public:
        F_RenderRegistryRuntime(const F_RenderRegistryRuntimeBuildParams& BuildParams);
        ~F_RenderRegistryRuntime() override;

    public:
        TS<A_RHITemplateRuntime> QueryTemplateRuntime(F_RHITemplateHashCode HashCode);
        TS<A_RHITemplateRuntime> GetOrActivate(F_RHITemplateHashCode HashCode);
    };
}