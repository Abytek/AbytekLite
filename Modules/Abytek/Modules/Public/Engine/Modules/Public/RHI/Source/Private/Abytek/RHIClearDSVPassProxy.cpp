#include "Abytek/RHIClearDSVPassProxy.hpp"
#include "Abytek/RHIClearDSVPass.hpp"
#include "Abytek/RHIResourceViewProxy.hpp"
#include "Abytek/DirectX12/RHIClearDSVPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIClearDSVPassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIClearDSVPassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIClearDSVPassProxy::Build(const TW_Valid<A_RHIClearDSVPass>& Pass)
    {
        A_RHIPassProxy::Build(Pass);
        _DSVProxy = Pass->GetDSV()->GetProxy().FastCast<A_RHIResourceViewProxy>();
        _Flags = Pass->GetFlags();
        _Depth = Pass->GetDepth();
        _Stencil = Pass->GetStencil();
    }   
    void A_RHIClearDSVPassProxy::Release()
    {
        _Stencil = 0;
        _Depth = 0.0f;
        _Flags = E_RHIClearDSVFlag::NONE;
        _DSVProxy = {};
        A_RHIPassProxy::Release();
    }
}