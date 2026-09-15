#include "Abytek/RHIClearUAVFloatPass.hpp"
#include "Abytek/DirectX12/RHIClearUAVFloatPass.hpp"
#include "Abytek/RHIClearUAVFloatPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIClearUAVFloatPass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIClearUAVFloatPass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIClearUAVFloatPass::Build(const F_RHIClearUAVFloatPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.UAV) << "Invalid UAV to clear";
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.UAV->GetArchetype() == E_RHIResourceViewArchetype::TEXTURE_VIEW) << "Invalid UAV resource view archetype";
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.UAV->GetTextureViewAspect().UAVClearable) << "Requires UAVClearable option in UAV texture view aspect";
        _UAV = BuildParams.UAV;
        _Value = BuildParams.Value;
    }   
    void A_RHIClearUAVFloatPass::Release()
    {
        _Value = F_Vector4_F32::Zero();
        _UAV = {};
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHIClearUAVFloatPass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIClearUAVFloatPassProxy>(ABYTEK_WTHIS());
    }
}
