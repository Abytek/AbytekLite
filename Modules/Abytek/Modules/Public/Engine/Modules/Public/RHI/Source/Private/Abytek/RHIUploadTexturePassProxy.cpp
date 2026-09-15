#include "Abytek/RHIUploadTexturePassProxy.hpp"
#include "Abytek/RHIUploadTexturePass.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIUploadTexturePassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIUploadTexturePassProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIUploadTexturePassProxy>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIUploadTexturePassProxy::Build(const TW_Valid<A_RHIUploadTexturePass>& Pass)
    {
        A_RHIPassProxy::Build(Pass);
        _TextureDataView = Pass->GetTextureDataView();;
        _TextureProxy = Pass->GetTexture()->GetProxy().FastCast<A_RHIResourceProxy>();
    }   
    void A_RHIUploadTexturePassProxy::Release()
    {
        _TextureProxy = {};
        _TextureDataView = {};
        A_RHIPassProxy::Release();
    }
}