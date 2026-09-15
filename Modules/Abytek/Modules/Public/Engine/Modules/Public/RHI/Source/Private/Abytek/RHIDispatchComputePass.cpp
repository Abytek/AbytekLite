#include "Abytek/RHIDispatchComputePass.hpp"
#include "Abytek/DirectX12/RHIDispatchComputePass.hpp"
#include "Abytek/RHIDispatchComputePassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIDispatchComputePass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
    switch (RHISubsystem->GetActiveAPI())
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
    case E_RHIAPI::DIRECTX12:
        return TU<F_DirectX12RHIDispatchComputePass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIDispatchComputePass::Build(const F_RHIDispatchComputePassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        A_RHIHasGeneralPipeline::Build(BuildParams);
        
        _NumThreadGroups = BuildParams.NumThreadGroups;
        _Indirect = BuildParams.Indirect;
    }   
    void A_RHIDispatchComputePass::Release()
    {
        _Indirect = {};
        _NumThreadGroups = F_Vector3_U32::Zero();
        
        A_RHIHasGeneralPipeline::Release();
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHIDispatchComputePass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIDispatchComputePassProxy>(ABYTEK_WTHIS());
    }
}
