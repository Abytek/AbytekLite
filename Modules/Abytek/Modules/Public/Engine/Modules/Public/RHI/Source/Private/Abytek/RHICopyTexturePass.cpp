#include "Abytek/RHICopyTexturePass.hpp"
#include "Abytek/DirectX12/RHICopyTexturePass.hpp"
#include "Abytek/RHICopyTexturePassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHICopyTexturePass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHICopyTexturePass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHICopyTexturePass::Build(const F_RHICopyTexturePassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        _SrcTexture = BuildParams.SrcTexture;
        _DestTexture = BuildParams.DestTexture;
        _SrcElementSet = BuildParams.SrcElementSet;
        _DstElementSet = BuildParams.DstElementSet;
    }   
    void A_RHICopyTexturePass::Release()
    {
        _DstElementSet = {};
        _SrcElementSet = {};
        _DestTexture = {};
        _SrcTexture = {};
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHICopyTexturePass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHICopyTexturePassProxy>(ABYTEK_WTHIS());
    }
}
