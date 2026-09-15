#include "Abytek/RHIClearRTVPass.hpp"
#include "Abytek/DirectX12/RHIClearRTVPass.hpp"
#include "Abytek/RHIClearRTVPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIClearRTVPass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIClearRTVPass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIClearRTVPass::Build(const F_RHIClearRTVPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.RTV) << "Invalid RTV to clear";
        _RTV = BuildParams.RTV;
        _Color = BuildParams.Color;
    }   
    void A_RHIClearRTVPass::Release()
    {
        _Color = F_Vector4_F32::Zero();
        _RTV = {};
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHIClearRTVPass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIClearRTVPassProxy>(ABYTEK_WTHIS());
    }
}
