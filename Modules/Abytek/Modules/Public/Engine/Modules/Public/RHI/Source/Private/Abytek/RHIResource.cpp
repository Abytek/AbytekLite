#include "Abytek/RHIResource.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIResource)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIResource>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()

    void A_RHIResource::Release()
    {
        _RTASAspect = {};
        _TextureAspect = {};
        _BufferAspect = {};
            
        _Format = E_RHIFormat::NONE;
        _AccessCapabilities = {};
        _InitialAccess = {};
        _AdditionalFlags = E_RHIResourceAdditionalFlag::DEFAULT;

        _Archetype = E_RHIResourceArchetype::NONE;

        A_RHIContextChild::Release();
    }

    void A_RHIResource::Build(const F_RHIBufferBuildParams& BuildParams)
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "Not implemented";
    }
    void A_RHIResource::Build(const F_RHITextureBuildParams& BuildParams)
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "Not implemented";
    }
    void A_RHIResource::Build(const F_RHIRTASBuildParams& BuildParams)
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "Not implemented";
    }

    void A_RHIResource::Build(
        const F_RHIResourceBuildParamsBase& BuildParamsBase,
        const F_RHIBufferAspect& BufferAspect
    )
    {
        Build(BuildParamsBase);
            
        InjectArchetype() |= E_RHIResourceArchetype::BUFFER;
        InjectBufferAspect() = BufferAspect;
            
        GetProxy().FastCast<A_RHIResourceProxy>()->LateBuildBuffer();
    }
    void A_RHIResource::Build(
        const F_RHIResourceBuildParamsBase& BuildParamsBase,
        const F_RHITextureAspect& TextureAspect
    )
    {
        Build(BuildParamsBase);

        InjectArchetype() |= E_RHIResourceArchetype::TEXTURE;
        InjectTextureAspect() = TextureAspect;
            
        GetProxy().FastCast<A_RHIResourceProxy>()->LateBuildTexture();
    }
    void A_RHIResource::Build(
        const F_RHIResourceBuildParamsBase& BuildParamsBase,
        const F_RHIRTASAspect& RTASAspect
    )
    {
        Build(BuildParamsBase);

        InjectArchetype() |= E_RHIResourceArchetype::RTAS;
        InjectRTASAspect() = RTASAspect;
            
        GetProxy().FastCast<A_RHIResourceProxy>()->LateBuildRTAS();
    }

    void A_RHIResource::Build(const F_RHIResourceBuildParamsBase& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);

        _Format = BuildParams.Format;
        _AccessCapabilities = BuildParams.AccessCapabilities;
        _InitialAccess = BuildParams.InitialAccess;
        _StaticAccess = BuildParams.StaticAccess;
        _AdditionalFlags = BuildParams.AdditionalFlags;
        ABYTEK_ENGINE_RHI_ASSERT(!FlagHas(_AdditionalFlags, E_RHIResourceAdditionalFlag::STATIC)) << "E_RHIResourceAdditionalFlag::STATIC cannot be used as initial additional flag";
        ABYTEK_ENGINE_RHI_ASSERT(
            FlagHas(_AdditionalFlags, E_RHIResourceAdditionalFlag::STATIC_AFTER_SUBMIT)
            == static_cast<B8>(_StaticAccess)
        ) << "E_RHIResourceAdditionalFlag::STATIC_AFTER_SUBMIT requires StaticAccess";
            
        GetProxy().FastCast<A_RHIResourceProxy>()->LateBuildResource();
    }

    void A_RHIResource::ReleaseAfterSubmit()
    {
        InjectAdditionalFlags() |= E_RHIResourceAdditionalFlag::RELEASE_AFTER_SUBMIT;
    }

    void A_RHIResource::EnableStaticAfterSubmit(const F_RHIResourceAccess& StaticAccess)
    {
        if (FlagHas(_AdditionalFlags, E_RHIResourceAdditionalFlag::STATIC))
        {
            if (_StaticAccess)
            {
                if (*_StaticAccess == StaticAccess)
                {
                    return;
                }
            }
        }
        ABYTEK_ENGINE_RHI_ASSERT(
            RHIResourceCanUseAsStatic(StaticAccess, _AdditionalFlags)
        ) << "Cannot enable static on this resource access (requires E_RHIResourceAdditionalFlag::STATIC_ON_WRITE_ACCESS): " << F_NewLine {} << StaticAccess.GetInfoText(1);
        _StaticAccess = StaticAccess;
        _AdditionalFlags |= E_RHIResourceAdditionalFlag::STATIC_AFTER_SUBMIT;
    }
    void A_RHIResource::DisableStatic()
    {
        _StaticAccess = {};
        _AdditionalFlags = FlagRemove(_AdditionalFlags, E_RHIResourceAdditionalFlag::STATIC_AFTER_SUBMIT);
        _AdditionalFlags = FlagRemove(_AdditionalFlags, E_RHIResourceAdditionalFlag::STATIC);
    }

    TS<A_RHIContextChildProxy> A_RHIResource::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIResourceProxy>(ABYTEK_WTHIS());
    }
}
