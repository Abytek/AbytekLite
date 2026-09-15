#pragma once

#include "Abytek/RenderProxy.hpp"
#include "Abytek/RenderPack.hpp"


namespace Abytek
{
    class F_RenderRegistry;
    class F_RenderRegistryRuntime;
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderPackProxy : public A_RenderProxy, public A_RenderPackTemplateMap
    {
    public:
        friend class F_RenderPack;
        
    private:
        TS<F_RenderRegistry> _Registry;
        TS<F_RenderRegistryRuntime> _RegistryRuntime;
        TF_Vector<TS<A_RHITemplateRuntime>> _TemplateRuntimes;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRegistry() const noexcept
        {
            return _Registry;
        }
        ABYTEK_FORCE_INLINE const auto& GetRegistryRuntime() const noexcept
        {
            return _RegistryRuntime;
        }
        ABYTEK_FORCE_INLINE const auto& GetTemplateRuntimes() const noexcept
        {
            return _TemplateRuntimes;
        }
        
    public:
        F_RenderPackProxy(const TW_Valid<A_Renderable>& Owner);
        ~F_RenderPackProxy() override;
        
    protected:
        void OnInit_RenderTask() override;
        void OnRelease_RenderTask() override;
        
    protected:
        void OnCreateRenderState_RenderTask() override;
        void OnDestroyRenderState_RenderTask() override;
        
    public:
        TS<A_RHITemplateRuntime> GetTemplateRuntime(F_RHITemplateHashCode HashCode);
        
    protected:
        void OnAddTemplate(const TS<A_RHITemplate>& Template) override;
    };
}