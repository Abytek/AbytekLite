#include "Abytek/RHIRayTracePass.hpp"
#include "Abytek/DirectX12/RHIRayTracePass.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIRayTracePass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
    switch (RHISubsystem->GetActiveAPI())
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
    case E_RHIAPI::DIRECTX12:
        return TU<F_DirectX12RHIRayTracePass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIRayTracePass::Build(const F_RHIRayTracePassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        A_RHIHasGeneralPipeline::Build(BuildParams);
    }   
    void A_RHIRayTracePass::Release()
    {
        A_RHIHasGeneralPipeline::Release();
        A_RHIPass::Release();
    }
}