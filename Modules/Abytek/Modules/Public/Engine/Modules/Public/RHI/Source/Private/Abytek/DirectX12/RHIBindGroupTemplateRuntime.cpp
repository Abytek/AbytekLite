#include "Abytek/DirectX12/RHIBindGroupTemplateRuntime.hpp"
#include "Abytek/DirectX12/RHIBindGroupTemplateRuntimeProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIBindGroupTemplateRuntime::Build(const F_RHIBindGroupTemplateRuntimeBuildParams& BuildParams)
    {
        A_RHIBindGroupTemplateRuntime::Build(BuildParams);
    }
    void F_DirectX12RHIBindGroupTemplateRuntime::Release()
    {
        A_RHIBindGroupTemplateRuntime::Release();
    }

    void F_DirectX12RHIBindGroupTemplateRuntime::FinalizeActivation()
    {
        A_RHIBindGroupTemplateRuntime::FinalizeActivation();
        GetProxy().FastCast<F_DirectX12RHIBindGroupTemplateRuntimeProxy>()->LateBuildBindGroupTemplateRuntime_DirectX12();
    }
}
#endif