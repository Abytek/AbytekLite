#include "Abytek/RHIClearUAVUIntPass.hpp"
#include "Abytek/DirectX12/RHIClearUAVUIntPass.hpp"
#include "Abytek/RHIClearUAVUIntPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIClearUAVUIntPass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIClearUAVUIntPass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIClearUAVUIntPass::Build(const F_RHIClearUAVUIntPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.UAV) << "Invalid UAV to clear";
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.UAV->GetArchetype() == E_RHIResourceViewArchetype::TEXTURE_VIEW) << "Invalid UAV resource view archetype";
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.UAV->GetTextureViewAspect().UAVClearable) << "Requires UAVClearable option in UAV texture view aspect";
        _UAV = BuildParams.UAV;
        _Value = BuildParams.Value;
    }   
    void A_RHIClearUAVUIntPass::Release()
    {
        _Value = F_Vector4_F32::Zero();
        _UAV = {};
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHIClearUAVUIntPass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIClearUAVUIntPassProxy>(ABYTEK_WTHIS());
    }
}
