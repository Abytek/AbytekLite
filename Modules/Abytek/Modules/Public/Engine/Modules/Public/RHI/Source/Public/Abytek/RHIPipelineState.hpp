#pragma once

#include "Abytek/RHIContextChild.hpp"


namespace Abytek
{
    class A_RHIPipelineStateTemplateRuntime;
    
    enum class E_RHIPipelineStateType
    {
        NONE,
        GRAPHICS,
        COMPUTE,
        WORK_GRAPH
    };

    /*struct F_RHIPipelineStateBuildParams : F_RHIContextChildBuildParams
    {
         TS<A_RHIPipelineStateTemplateRuntime> TemplateRuntime;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIPipelineState : public A_RHIContextChild
    {
    private:
         TS<A_RHIPipelineStateTemplateRuntime> _TemplateRuntime;

    public:
        ABYTEK_FORCE_INLINE const auto& GetTemplateRuntime() const noexcept
        {
            return _TemplateRuntime;
        }
        ABYTEK_FORCE_INLINE auto& InjectTemplateRuntime() noexcept
        {
            return _TemplateRuntime;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIPipelineState);
        virtual void Build(const F_RHIPipelineStateBuildParams& BuildParams);
        virtual void Release();

    protected:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;
    };*/
    
    using A_RHIPipelineState = A_RHIPipelineStateTemplateRuntime;
}