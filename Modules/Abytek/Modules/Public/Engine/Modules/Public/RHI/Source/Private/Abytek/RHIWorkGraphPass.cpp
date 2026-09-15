#include "Abytek/RHIWorkGraphPass.hpp"
#include "Abytek/DirectX12/RHIWorkGraphPass.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIWorkGraphPass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
    switch (RHISubsystem->GetActiveAPI())
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
    case E_RHIAPI::DIRECTX12:
        return TU<F_DirectX12RHIWorkGraphPass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIWorkGraphPass::Build(const F_RHIWorkGraphPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        A_RHIHasGeneralPipeline::Build(BuildParams);
    }   
    void A_RHIWorkGraphPass::Release()
    {
        A_RHIHasGeneralPipeline::Release();
        A_RHIPass::Release();
    }
}