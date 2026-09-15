#include "Abytek/RHICopyBufferPass.hpp"
#include "Abytek/DirectX12/RHICopyBufferPass.hpp"

#include "Abytek/RHICopyBufferPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHICopyBufferPass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
    switch (RHISubsystem->GetActiveAPI())
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
    case E_RHIAPI::DIRECTX12:
        return TU<F_DirectX12RHICopyBufferPass>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHICopyBufferPass::Build(const F_RHICopyBufferPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        _SrcBuffer = BuildParams.SrcBuffer;
        _SrcOffsetInBytes = BuildParams.SrcOffsetInBytes;
        _DstBuffer = BuildParams.DstBuffer;
        _DstOffsetInBytes = BuildParams.DstOffsetInBytes;
        _SizeInBytes = BuildParams.SizeInBytes;
    }   
    void A_RHICopyBufferPass::Release()
    {
        _SizeInBytes = 0;
        _DstOffsetInBytes = 0;
        _DstBuffer = {};
        _SrcOffsetInBytes = 0;
        _SrcBuffer = {};
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHICopyBufferPass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHICopyBufferPassProxy>(ABYTEK_WTHIS());
    }
}
