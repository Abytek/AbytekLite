#include "Abytek/RHIReadbackBufferPass.hpp"
#include "Abytek/DirectX12/RHIReadbackBufferPass.hpp"

#include "Abytek/RHIReadbackBufferPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIReadbackBufferPass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
    switch (RHISubsystem->GetActiveAPI())
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
    case E_RHIAPI::DIRECTX12:
        return TU<F_DirectX12RHIReadbackBufferPass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHIReadbackBufferPass::Build(const F_RHIReadbackBufferPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        _Buffer = BuildParams.Buffer;
        _SizeInBytes = BuildParams.SizeInBytes;
        _OffsetInBytes = BuildParams.OffsetInBytes;
        _Callback = BuildParams.Callback;
            
        if (_SizeInBytes == 0)
        {
            _SizeInBytes = _Buffer->GetBufferAspect().SizeInBytes;
        }
            
        ABYTEK_ENGINE_RHI_ASSERT(_Callback) << "Invalid readback buffer callback";
    }   
    void A_RHIReadbackBufferPass::Release()
    {
        _Callback = {};
        _OffsetInBytes = 0;
        _SizeInBytes = 0;
        _Buffer = {};
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHIReadbackBufferPass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIReadbackBufferPassProxy>(ABYTEK_WTHIS());
    }

    B8 A_RHIReadbackBufferPass::CanDetachCopyPass()
    {
        return false;
    }
    void A_RHIReadbackBufferPass::DetachCopyPass(I_RHISubmissionItemContainer& SubmissionItemContainer)
    {
        ABYTEK_LOG_FATAL() << "Not supported";
    }
}
