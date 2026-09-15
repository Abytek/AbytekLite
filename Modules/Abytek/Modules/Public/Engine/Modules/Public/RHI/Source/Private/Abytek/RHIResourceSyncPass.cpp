#include "Abytek/RHIResourceSyncPass.hpp"
#include "Abytek/DirectX12/RHIResourceSyncPass.hpp"
#include "Abytek/RHIResourceSyncPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIResourceSyncPass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIResourceSyncPass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIResourceSyncPass::Build(const F_RHIResourceSyncPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.Resource) << "Invalid resource to sync";
        _Resource = BuildParams.Resource;
        _Access = BuildParams.Access;
    }   
    void A_RHIResourceSyncPass::Release()
    {
        _Access = {};
        _Resource = {};
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHIResourceSyncPass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIResourceSyncPassProxy>(ABYTEK_WTHIS());
    }
}
