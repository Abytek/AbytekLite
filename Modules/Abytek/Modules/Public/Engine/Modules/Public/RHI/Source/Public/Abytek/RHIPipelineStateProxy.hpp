#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIPipelineState.hpp"


namespace Abytek
{
    class A_RHIPipelineStateTemplateRuntimeProxy;
    
    /*
    
    class ABYTEK_ENGINE_RHI_API A_RHIPipelineStateProxy :
        public A_RHIContextChildProxy
    {
    private:
        TS<A_RHIPipelineStateTemplateRuntimeProxy> _TemplateRuntimeProxy;
        
    public:
        ABYTEK_FORCE_INLINE auto GetPipelineState() const noexcept
        {
            return GetContextChild().FastCast<A_RHIPipelineState>();
        }
        
        ABYTEK_FORCE_INLINE const auto& GetTemplateRuntimeProxy() const noexcept
        {
            return _TemplateRuntimeProxy;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIPipelineStateProxy);
        void LateBuildPipelineState();
        virtual void Release() override;
    };*/
    
    using A_RHIPipelineStateProxy = A_RHIPipelineStateTemplateRuntimeProxy;
}
