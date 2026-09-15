#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIPipelineState.hpp"


namespace Abytek
{
    class A_RHIBindGroup;
    
    struct F_RHIHasGeneralPipelineBuildParams
    {
        TS<A_RHIPipelineState> PipelineState;
        F_RHIBindGroupSet BindGroups;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIHasGeneralPipeline
    {
    private:
        TS<A_RHIPipelineState> _PipelineState;
        F_RHIBindGroupSet _BindGroups;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPipelineState() const noexcept
        {
            return _PipelineState;
        }
        ABYTEK_FORCE_INLINE auto& InjectPipelineState() noexcept
        {
            return _PipelineState;
        }
        ABYTEK_FORCE_INLINE const auto& GetBindGroups() const noexcept
        {
            return _BindGroups;
        }
        ABYTEK_FORCE_INLINE auto& InjectBindGroups() noexcept
        {
            return _BindGroups;
        }
        
    public:
        A_RHIHasGeneralPipeline() = default;
        virtual ~A_RHIHasGeneralPipeline() = default;
        void Build(const F_RHIHasGeneralPipelineBuildParams& BuildParams);
        void Release();
    };
}