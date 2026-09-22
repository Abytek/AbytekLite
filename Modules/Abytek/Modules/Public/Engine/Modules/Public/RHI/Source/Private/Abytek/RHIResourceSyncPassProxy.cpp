#include "Abytek/RHIResourceSyncPassProxy.hpp"
#include "Abytek/RHIResourceSyncPass.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIResourceSyncPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIResourceSyncPassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIResourceSyncPassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIResourceSyncPassProxy::Build(const TW_Valid<A_RHIResourceSyncPass>& Pass)
    {
        BuildPassProxy(Pass);
        _ResourceProxy = Pass->GetResource()->GetProxy().FastCast<A_RHIResourceProxy>();
        _Access = Pass->GetAccess();
    }   
    void A_RHIResourceSyncPassProxy::Release()
    {
        _Access = {};
        _ResourceProxy = {};
        A_RHIPassProxy::Release();
    }
}