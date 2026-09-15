#include "Abytek/RHIClearDSVPass.hpp"
#include "Abytek/RHIClearDSVPassProxy.hpp"
#include "Abytek/DirectX12/RHIClearDSVPass.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIClearDSVPass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
    switch (RHISubsystem->GetActiveAPI())
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
    case E_RHIAPI::DIRECTX12:
        return TU<F_DirectX12RHIClearDSVPass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIClearDSVPass::Build(const F_RHIClearDSVPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.DSV) << "Invalid DSV to clear";
        _DSV = BuildParams.DSV;
        _Flags = BuildParams.Flags;
        _Depth = BuildParams.Depth;
        _Stencil = BuildParams.Stencil;
    }   
    void A_RHIClearDSVPass::Release()
    {
        _Stencil = 0;
        _Depth = 0.0f;
        _Flags = E_RHIClearDSVFlag::NONE;
        _DSV = {};
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHIClearDSVPass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIClearDSVPassProxy>(ABYTEK_WTHIS());
    }
}
