#include "Abytek/RHIReadbackTexturePassProxy.hpp"
#include "Abytek/RHIReadbackTexturePass.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIReadbackTexturePassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIReadbackTexturePassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIReadbackTexturePassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIReadbackTexturePassProxy::Build(const TW_Valid<A_RHIReadbackTexturePass>& Pass)
    {
        A_RHIPassProxy::Build(Pass);
        _TextureProxy = Pass->GetTexture()->GetProxy().FastCast<A_RHIResourceProxy>();
        _Callback = Pass->GetCallback();
    }   
    void A_RHIReadbackTexturePassProxy::Release()
    {
        _Callback = {};
        _TextureProxy = {};
        A_RHIPassProxy::Release();
    }
}