#include "Abytek/RHIHasGeneralPipeline.hpp"


namespace Abytek
{
    void A_RHIHasGeneralPipeline::Build(const F_RHIHasGeneralPipelineBuildParams& BuildParams)
    {
        _PipelineState = BuildParams.PipelineState;
        _BindGroups = BuildParams.BindGroups;
        
        ABYTEK_ENGINE_RHI_ASSERT(_PipelineState) << "Invalid pipeline state";
    }
    void A_RHIHasGeneralPipeline::Release()
    {
        _BindGroups = {};
        _PipelineState = {};
    }
}