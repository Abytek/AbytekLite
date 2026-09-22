#include "Abytek/RHIUploadTexturePass.hpp"
#include "Abytek/DirectX12/RHIUploadTexturePass.hpp"

#include "Abytek/RHIUploadTexturePassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIUploadTexturePass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIUploadTexturePass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIUploadTexturePass::Build(const F_RHIUploadTexturePassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        _TextureDataView = BuildParams.TextureDataView;
        _Texture = BuildParams.Texture;
            
        ABYTEK_ENGINE_RHI_ASSERT(_Texture) << "Invalid buffer to upload into";
    }   
    void A_RHIUploadTexturePass::Release()
    {
        _Texture = {};
        _TextureDataView = {};
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHIUploadTexturePass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIUploadTexturePassProxy>(ABYTEK_WTHIS());
    }

    B8 A_RHIUploadTexturePass::CanDetachCopyPass()
    {
        return false;
    }
    void A_RHIUploadTexturePass::DetachCopyPass(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        ABYTEK_LOG_FATAL() << "Not supported";
    }
}
