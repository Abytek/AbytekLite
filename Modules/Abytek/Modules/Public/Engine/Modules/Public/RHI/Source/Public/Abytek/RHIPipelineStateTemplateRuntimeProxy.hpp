#pragma once

#include "Abytek/RHITemplateRuntime.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntime.hpp"


namespace Abytek
{
    class A_RHIBindGroupTemplateRuntimeProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIPipelineStateTemplateRuntimeProxy :
        public A_RHITemplateRuntimeProxy
    {
    private:
        TF_Vector<TS<A_RHIBindGroupTemplateRuntimeProxy>> _BindGroupTemplateRuntimeProxies;

    public:
        ABYTEK_FORCE_INLINE auto GetPipelineStateTemplateRuntime() const noexcept
        {
            return GetContextChild().FastCast<A_RHIPipelineStateTemplateRuntime>();
        }
        
        ABYTEK_FORCE_INLINE const auto& GetBindGroupTemplateRuntimeProxies() const noexcept
        {
            return _BindGroupTemplateRuntimeProxies;
        }
        ABYTEK_FORCE_INLINE auto& InjectBindGroupTemplateRuntimeProxies() noexcept
        {
            return _BindGroupTemplateRuntimeProxies;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIPipelineStateTemplateRuntimeProxy);
        void LateBuildPipelineStateTemplateRuntime();
        virtual void Release() override;
    };
}
