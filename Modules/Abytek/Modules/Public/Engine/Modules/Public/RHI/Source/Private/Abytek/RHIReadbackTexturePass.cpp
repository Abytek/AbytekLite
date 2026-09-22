#include "Abytek/RHIReadbackTexturePass.hpp"
#include "Abytek/DirectX12/RHIReadbackTexturePass.hpp"

#include "Abytek/RHIReadbackTexturePassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIReadbackTexturePass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIReadbackTexturePass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIReadbackTexturePass::Build(const F_RHIReadbackTexturePassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        _Texture = BuildParams.Texture;
        _Callback = BuildParams.Callback;
        ABYTEK_ENGINE_RHI_ASSERT(_Callback) << "Invalid readback buffer callback";
    }   
    void A_RHIReadbackTexturePass::Release()
    {
        _Callback = {};
        _Texture = {};
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHIReadbackTexturePass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIReadbackTexturePassProxy>(ABYTEK_WTHIS());
    }

    F_RHIReadbackTextureCallback A_RHIReadbackTexturePass::MoveCallback()
    {
        return ABYTEK_MOVE(_Callback);
    }
}
