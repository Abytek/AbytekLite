#include "Abytek/RHIClearRTVPassProxy.hpp"
#include "Abytek/RHIClearRTVPass.hpp"
#include "Abytek/RHIResourceViewProxy.hpp"
#include "Abytek/DirectX12/RHIClearRTVPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIClearRTVPassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIClearRTVPassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIClearRTVPassProxy::Build(const TW_Valid<A_RHIClearRTVPass>& Pass)
    {
        BuildPassProxy(Pass);
        _RTVProxy = Pass->GetRTV()->GetProxy().FastCast<A_RHIResourceViewProxy>();
        _Color = Pass->GetColor();
    }   
    void A_RHIClearRTVPassProxy::Release()
    {
        _Color = F_Vector4_F32::Zero();
        _RTVProxy = {};
        A_RHIPassProxy::Release();
    }
}