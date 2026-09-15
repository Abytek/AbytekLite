#include "Abytek/RHIClearUAVFloatPassProxy.hpp"
#include "Abytek/RHIClearUAVFloatPass.hpp"
#include "Abytek/RHIResourceViewProxy.hpp"
#include "Abytek/DirectX12/RHIClearUAVFloatPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIClearUAVFloatPassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIClearUAVFloatPassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIClearUAVFloatPassProxy::Build(const TW_Valid<A_RHIClearUAVFloatPass>& Pass)
    {
        A_RHIPassProxy::Build(Pass);
        _UAVProxy = Pass->GetUAV()->GetProxy().FastCast<A_RHIResourceViewProxy>();
        _Value = Pass->GetValue();
    }   
    void A_RHIClearUAVFloatPassProxy::Release()
    {
        _Value = F_Vector4_F32::Zero();
        _UAVProxy = {};
        A_RHIPassProxy::Release();
    }
}