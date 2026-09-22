#include "Abytek/RHICopyTexturePassProxy.hpp"
#include "Abytek/RHICopyTexturePass.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHICopyTexturePassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHICopyTexturePassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHICopyTexturePassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHICopyTexturePassProxy::Build(const TW_Valid<A_RHICopyTexturePass>& Pass)
    {
        BuildPassProxy(Pass);
        _SrcTextureProxy = Pass->GetSrcTexture()->GetProxy().FastCast<A_RHIResourceProxy>();
        _DestTextureProxy = Pass->GetDestTexture()->GetProxy().FastCast<A_RHIResourceProxy>();
        _SrcElementSet = Pass->GetSrcElementSet();
        _DstElementSet = Pass->GetDstElementSet();
    }   
    void A_RHICopyTexturePassProxy::Release()
    {
        _DstElementSet = {};
        _SrcElementSet = {};
        _DestTextureProxy = {};
        _SrcTextureProxy = {};
        A_RHIPassProxy::Release();
    }
}