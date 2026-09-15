#include "Abytek/DirectX12/RHIPipelineStateTemplateRuntime.hpp"
#include "Abytek/RHITemplateRuntimeDatabase.hpp"
#include "Abytek/DirectX12/RHIPipelineStateTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12/RHIProcess.hpp"
#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntime.hpp"
#include "Abytek/DirectX12Shared/RHIPipelineStateTemplate.hpp"
#include "Abytek/DirectX12Shared/RHIRootSignatureTemplate.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIPipelineStateTemplateRuntime::Build(const F_RHIPipelineStateTemplateRuntimeBuildParams& BuildParams)
    {
        A_RHIPipelineStateTemplateRuntime::Build(BuildParams);
    }
    void F_DirectX12RHIPipelineStateTemplateRuntime::Release()
    {
        _RootSignatureTemplateRuntime = {};
        A_RHIPipelineStateTemplateRuntime::Release();
    }

    void F_DirectX12RHIPipelineStateTemplateRuntime::FinalizeActivation()
    { 
        A_RHIPipelineStateTemplateRuntime::FinalizeActivation();
        
        _RootSignatureTemplateRuntime = GetDatabase()->GetOrActivateRuntime(
            GetTemplate().FastCast<F_DirectX12SharedRHIPipelineStateTemplate>()
            ->GetRootSignatureTemplate()
        ).FastCast<F_DirectX12RHIRootSignatureTemplateRuntime>();
        
        GetProxy().FastCast<F_DirectX12RHIPipelineStateTemplateRuntimeProxy>()->LateBuildPipelineStateTemplateRuntime_DirectX12();
        
        InitD3D12PipelineState();
    }

    void F_DirectX12RHIPipelineStateTemplateRuntime::InitD3D12PipelineState()
    {
        DirectX12RHIProcessQueries::Execution::F_InitPipelineState Query;
        Query.PipelineStateTemplateRuntimeProxy = GetProxy().FastCast<F_DirectX12RHIPipelineStateTemplateRuntimeProxy>();
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()->Queues.Execution.InitPipelineState.Push(Query);
    }
}
#endif