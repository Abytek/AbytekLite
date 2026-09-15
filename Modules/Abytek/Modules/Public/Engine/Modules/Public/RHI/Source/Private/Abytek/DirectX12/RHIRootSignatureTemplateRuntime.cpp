#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntime.hpp"
#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntimeProxy.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIProcess.hpp"
#include "Abytek/DirectX12Shared/RHIRootSignatureTemplate.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHIRootSignatureTemplateRuntime)
    void F_DirectX12RHIRootSignatureTemplateRuntime::Build(const F_DirectX12RHIRootSignatureTemplateRuntimeBuildParams& BuildParams)
    {
        A_RHITemplateRuntime::Build(BuildParams);
    }
    void F_DirectX12RHIRootSignatureTemplateRuntime::Release()
    {
        A_RHITemplateRuntime::Release();
    }

    void F_DirectX12RHIRootSignatureTemplateRuntime::FinalizeActivation()
    {
        A_RHITemplateRuntime::FinalizeActivation();
        GetProxy().FastCast<F_DirectX12RHIRootSignatureTemplateRuntimeProxy>()->LateBuildRootSignatureTemplateRuntime();
        InitD3D12RootSignature();
    }

    TS<A_RHIContextChildProxy> F_DirectX12RHIRootSignatureTemplateRuntime::CreateProxy()
    {
        return RACreateAndBuildShared<F_DirectX12RHIRootSignatureTemplateRuntimeProxy>(ABYTEK_WTHIS());
    }

    void F_DirectX12RHIRootSignatureTemplateRuntime::InitD3D12RootSignature()
    {
        DirectX12RHIProcessQueries::Execution::F_InitRootSignature Query;
        Query.RootSignatureTemplateRuntimeProxy = GetProxy().FastCast<F_DirectX12RHIRootSignatureTemplateRuntimeProxy>();
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()->Queues.Execution.InitRootSignature.Push(Query);
    }
}
#endif