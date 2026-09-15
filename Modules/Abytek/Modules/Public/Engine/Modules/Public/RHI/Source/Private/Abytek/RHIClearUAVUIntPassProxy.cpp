#include "Abytek/RHIClearUAVUIntPassProxy.hpp"
#include "Abytek/RHIClearUAVUIntPass.hpp"
#include "Abytek/RHIResourceViewProxy.hpp"
#include "Abytek/DirectX12/RHIClearUAVUIntPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIClearUAVUIntPassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIClearUAVUIntPassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIClearUAVUIntPassProxy::Build(const TW_Valid<A_RHIClearUAVUIntPass>& Pass)
    {
        A_RHIPassProxy::Build(Pass);
        _UAVProxy = Pass->GetUAV()->GetProxy().FastCast<A_RHIResourceViewProxy>();
        _Value = Pass->GetValue();
    }   
    void A_RHIClearUAVUIntPassProxy::Release()
    {
        _Value = F_Vector4_F32::Zero();
        _UAVProxy = {};
        A_RHIPassProxy::Release();
    }
}