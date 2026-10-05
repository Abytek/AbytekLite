#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"


namespace Abytek
{
    class A_RenderRegistryPort;
    class F_RenderRegistry;
    class F_RenderRegistryRuntime;
    struct F_RenderCoreRHIConfig;
    
    class ABYTEK_ENGINE_RENDER_CORE_API A_RenderResource : public A_Object
    {
    private:
        TS<F_RenderRegistryRuntime> _RenderRegistryRuntime;
#ifdef ABYTEK_ENGINE_RENDER_CORE_ENABLE_ASSERTIONS
        B8 _EnqueuedToInit = false;
        B8 _EnqueuedToRelease = false;
        
        TS<A_RenderRegistryPort> _LastRenderRegistryPort;
#endif
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRenderRegistryRuntime() const noexcept
        {
            return _RenderRegistryRuntime;
        }
        
    protected:
        A_RenderResource();
        
    public:
        ~A_RenderResource() override;
        
    public:
        void Init(const TS<A_RenderRegistryPort>& RenderRegistryPort);
        void Release(const TS<A_RenderRegistryPort>& RenderRegistryPort);
        
    protected:
        virtual void OnInit_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        virtual void OnRelease_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        const F_RHIFeatureSupports& GetRHIFeatureSupports() const noexcept;
    };
}
