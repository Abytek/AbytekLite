#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"


namespace Abytek
{
    class F_RenderRegistry;
    class F_RenderRegistryRuntime;
    
    enum class E_RenderRegistryRuntimeAccessFlag : U8
    {
        NONE = 0x0,
        LOAD = 0x1,
        DEFAULT = LOAD
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RenderRegistryRuntimeAccessFlag);
    
    struct F_RenderRegistryRuntimeBuildParams
    {
        TW<F_RenderRegistry> Registry;
        TS<A_RHIContext> Context;
    };
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderRegistryRuntime final : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_RenderRegistryRuntime)
        
    private:
        TW<F_RenderRegistry> _Registry;
        TS<A_RHIContext> _Context;
        F_RenderCoreRHIConfig _RHIConfig;
        TF_Vector<TS<F_RenderRegistryRuntime>> _Dependencies;
        TW<A_RHITemplateRuntimeDatabase> _TemplateRuntimeDatabase;
        
        F_AtomicFlag _IsActivated;

    public:
        ABYTEK_FORCE_INLINE const auto& GetRegistry() const noexcept
        {
            return _Registry;
        }
        ABYTEK_FORCE_INLINE const auto& GetContext() const noexcept
        {
            return _Context;
        }
        ABYTEK_FORCE_INLINE const auto& GetRHIConfig() const noexcept
        {
            return _RHIConfig;
        }
        ABYTEK_FORCE_INLINE const auto& GetDependencies() const noexcept
        {
            return _Dependencies;
        }
        ABYTEK_FORCE_INLINE const auto& GetTemplateRuntimeDatabase() const noexcept
        {
            return _TemplateRuntimeDatabase;
        }
        ABYTEK_FORCE_INLINE B8 IsActivated() const noexcept
        {
            return _IsActivated.test(boost::memory_order_acquire);
        }
        
    public:
        F_RenderRegistryRuntime(const F_RenderRegistryRuntimeBuildParams& BuildParams);
        ~F_RenderRegistryRuntime() override;
        
    public:
        void FinalizeActivation();
        
    public:
        void FinalizeActivationAndUnlock();

    public:
        ABYTEK_FORCE_INLINE void WaitForActivation() const noexcept
        {
            while (!IsActivated())
            {
                H_TaskUtilities::Switch();
            }
        }

    public:
        TS<A_RHITemplateRuntime> QueryTemplateRuntime(F_RHITemplateHashCode HashCode);
        TS<A_RHITemplateRuntime> GetOrActivate(F_RHITemplateHashCode HashCode);
    };
}