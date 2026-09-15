#include "Abytek/RHIBindGroupTemplateRuntime.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIBindGroupTemplateRuntime.hpp"
#include "Abytek/RHIBindGroupTemplateRuntimeProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIBindGroupTemplateRuntime)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIBindGroupTemplateRuntime>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT();
    void A_RHIBindGroupTemplateRuntime::Build(const F_RHIBindGroupTemplateRuntimeBuildParams& BuildParams)
    {
        A_RHITemplateRuntime::Build(BuildParams);
    }
    void A_RHIBindGroupTemplateRuntime::Release()
    {
        A_RHITemplateRuntime::Release();
    }

    void A_RHIBindGroupTemplateRuntime::FinalizeActivation()
    {
        A_RHITemplateRuntime::FinalizeActivation();
        GetProxy().FastCast<A_RHIBindGroupTemplateRuntimeProxy>()->LateBuildBindGroupTemplateRuntime();
    }

    TS<A_RHIContextChildProxy> A_RHIBindGroupTemplateRuntime::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIBindGroupTemplateRuntimeProxy>(ABYTEK_WTHIS());
    }
}
