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
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.Buffer) << "Invalid buffer";
        ABYTEK_ENGINE_RHI_ASSERT(FlagHas(BuildParams.Buffer->GetArchetype(), E_RHIResourceArchetype::BUFFER)) << "Requires buffer archetype";
        ABYTEK_ENGINE_RHI_ASSERT(
            FlagHas(
                BuildParams.Buffer->GetAccessCapabilities().GPU,
                E_RHIResourceGPUAccess::COPY_SOURCE
            )
        ) << "Requires copy source access";
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.OffsetInBytes <= BuildParams.Buffer->GetBufferAspect().SizeInBytes) << "Invalid offset in bytes";
        Sz ActualSizeInBytes = BuildParams.SizeInBytes;
        if (ActualSizeInBytes == 0)
        {
            ActualSizeInBytes = BuildParams.Buffer->GetBufferAspect().SizeInBytes - BuildParams.OffsetInBytes;
        }
        ABYTEK_ENGINE_RHI_ASSERT((BuildParams.OffsetInBytes + ActualSizeInBytes) <= BuildParams.Buffer->GetBufferAspect().SizeInBytes) << "Invalid size in bytes";
        _Buffer = BuildParams.Buffer;
        _SizeInBytes = ActualSizeInBytes;
        _OffsetInBytes = BuildParams.OffsetInBytes;
        _Callback = BuildParams.Callback;
            
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

    F_RHIReadbackBufferCallback A_RHIReadbackBufferPass::MoveCallback()
    {
        return ABYTEK_MOVE(_Callback);
    }
}
