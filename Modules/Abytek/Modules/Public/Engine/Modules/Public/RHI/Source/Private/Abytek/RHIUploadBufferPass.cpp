#include "Abytek/RHIUploadBufferPass.hpp"
#include "Abytek/DirectX12/RHIUploadBufferPass.hpp"

#include "Abytek/RHIUploadBufferPassProxy.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIUploadBufferPass)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIUploadBufferPass>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHIUploadBufferPass::Build(const F_RHIUploadBufferPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.Buffer) << "Invalid buffer";
        ABYTEK_ENGINE_RHI_ASSERT(FlagHas(BuildParams.Buffer->GetArchetype(), E_RHIResourceArchetype::BUFFER)) << "Requires buffer archetype";
        ABYTEK_ENGINE_RHI_ASSERT(
            FlagHas(
                BuildParams.Buffer->GetAccessCapabilities().GPU,
                E_RHIResourceGPUAccess::COPY_DEST
            )
        ) << "Requires copy dest access";
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.BufferDataView.size() <= BuildParams.Buffer->GetBufferAspect().SizeInBytes) << "Invalid buffer";
        _BufferDataView = BuildParams.BufferDataView;
        _Buffer = BuildParams.Buffer;
        _OffsetInBytes = BuildParams.OffsetInBytes;
    }   
    void A_RHIUploadBufferPass::Release()
    {
        _OffsetInBytes = 0;
        _Buffer = {};
        _BufferDataView = {};
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> A_RHIUploadBufferPass::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIUploadBufferPassProxy>(ABYTEK_WTHIS());
    }

    B8 A_RHIUploadBufferPass::CanDetachCopyPass()
    {
        return false;
    }
    void A_RHIUploadBufferPass::DetachCopyPass(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        ABYTEK_LOG_FATAL() << "Not supported";
    }
}
