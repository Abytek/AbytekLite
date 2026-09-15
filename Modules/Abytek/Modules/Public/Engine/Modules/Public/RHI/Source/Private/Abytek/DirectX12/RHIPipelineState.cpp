#include "Abytek/DirectX12/RHIPipelineState.hpp"
#include "Abytek/RHIPipelineStateTemplate.hpp"
#include "Abytek/DirectX12/RHIContext.hpp"
#include "Abytek/DirectX12/RHIPipelineStateProxy.hpp"
#include "Abytek/DirectX12/RHIProcess.hpp"
#include "Abytek/DirectX12Shared/RHIRootSignatureTemplate.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    /*void F_DirectX12RHIPipelineState::Build(const F_RHIPipelineStateBuildParams& BuildParams)
    {
        A_RHIPipelineState::Build(BuildParams);
        GetProxy().FastCast<F_DirectX12RHIPipelineStateProxy>()->LateBuildPipelineState_DirectX12();
        
        InitD3D12PipelineState();
    }
    void F_DirectX12RHIPipelineState::Release()
    {
        A_RHIPipelineState::Release();
    }

    void F_DirectX12RHIPipelineState::InitD3D12PipelineState()
    {
        DirectX12RHIProcessQueries::Execution::F_InitPipelineState Query;
        Query.PipelineStateProxy = GetProxy().FastCast<F_DirectX12RHIPipelineStateProxy>();
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()->Queues.Execution.InitPipelineState.Push(Query);
    }*/
}
#endif