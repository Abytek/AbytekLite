#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIResourceProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIResourceProxy>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHIResourceProxy::LateBuildResource()
    {
        auto Resource = GetResource();
            
        _Format = Resource->GetFormat();
        _AccessCapabilities = Resource->GetAccessCapabilities();
        _InitialAccess = Resource->GetInitialAccess();
        _StaticAccess = Resource->GetStaticAccess();
        _AdditionalFlags = Resource->GetAdditionalFlags();
    }
    void A_RHIResourceProxy::LateBuildBuffer()
    {
        _Archetype |= E_RHIResourceArchetype::BUFFER;
        _BufferAspect = GetResource()->GetBufferAspect();
    }
    void A_RHIResourceProxy::LateBuildTexture()
    {
        _Archetype |= E_RHIResourceArchetype::TEXTURE;
        _TextureAspect = GetResource()->GetTextureAspect();
    }
    void A_RHIResourceProxy::LateBuildRTAS()
    {
        _Archetype |= E_RHIResourceArchetype::RTAS;
        _RTASAspect = GetResource()->GetRTASAspect();
    }

    void A_RHIResourceProxy::Release()
    {
        A_RHIContextChildProxy::Release();
    }
}
