#include "Abytek/RHIResourceView.hpp"
#include "Abytek/RHIResourceViewProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIResourceView.hpp"

#include "Abytek/RHIResourceProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIResourceView)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIResourceView>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHIResourceView::Release()
    {
        _BindlessId = INVALID_RHI_BINDLESS_ID;
            
        _Archetype = E_RHIResourceViewArchetype::NONE;
        _BufferViewAspect = {};
        _TextureViewAspect = {};
        _RTASViewAspect = {};
            
        _AllowBindless = false;
        _Access = {};
        _Format = E_RHIFormat::NONE;
        _Resource = {};
            
        A_RHIContextChild::Release();
    }

    void A_RHIResourceView::Build(const F_RHIBufferViewBuildParams& BuildParams)
    {
        A_RHIResourceView::Build((const F_RHIResourceViewBuildParamsBase&)BuildParams);

        InjectArchetype() |= E_RHIResourceViewArchetype::BUFFER_VIEW;
        InjectBufferViewAspect() = BuildParams.BufferViewAspect;
            
        GetProxy().FastCast<A_RHIResourceViewProxy>()->LateBuildBufferView();
    }
    void A_RHIResourceView::Build(const F_RHITextureViewBuildParams& BuildParams)
    {
        A_RHIResourceView::Build((const F_RHIResourceViewBuildParamsBase&)BuildParams);

        InjectArchetype() |= E_RHIResourceViewArchetype::TEXTURE_VIEW;
        InjectTextureViewAspect() = BuildParams.TextureViewAspect;
            
        GetProxy().FastCast<A_RHIResourceViewProxy>()->LateBuildTextureView();
    }
    void A_RHIResourceView::Build(const F_RHIRTASViewBuildParams& BuildParams)
    {
        A_RHIResourceView::Build((const F_RHIResourceViewBuildParamsBase&)BuildParams);

        InjectArchetype() |= E_RHIResourceViewArchetype::RTAS_VIEW;
        InjectRTASViewAspect() = BuildParams.RTASViewAspect;
            
        GetProxy().FastCast<A_RHIResourceViewProxy>()->LateBuildRTASView();
    }

    void A_RHIResourceView::Build(const F_RHIResourceViewBuildParamsBase& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);

        _Resource = BuildParams.Resource;
        _Format = BuildParams.Format;
        _Access = BuildParams.Access;
        _AllowBindless = BuildParams.AllowBindless;
        ABYTEK_ENGINE_RHI_ASSERT(_Access) << "Invalid resource access";
            
        GetProxy().FastCast<A_RHIResourceViewProxy>()->LateBuildResourceView();
    }

    void A_RHIResourceView::Build(const TS_Valid<A_RHIResource>& Resource, const F_RHIResourceAccess& Access)
    {
        auto ResourceArchetype = Resource->GetArchetype();
        if (FlagHas(ResourceArchetype, E_RHIResourceArchetype::BUFFER))
        {
            const auto& BufferAspect = Resource->GetBufferAspect();
            
            F_RHIBufferViewBuildParams BuildParams;
            BuildParams.Resource = Resource;
            BuildParams.Format = Resource->GetFormat();
            BuildParams.Access = Access;
            BuildParams.BufferViewAspect.OffsetInBytes = 0;
            BuildParams.BufferViewAspect.SizeInBytes = BufferAspect.SizeInBytes;
            BuildParams.BufferViewAspect.StrideInBytes = BufferAspect.StrideInBytes;
            Build(BuildParams);
        }
        else if (FlagHas(ResourceArchetype, E_RHIResourceArchetype::TEXTURE))
        {
            const auto& TextureAspect = Resource->GetTextureAspect();
            
            F_RHITextureViewBuildParams BuildParams;
            BuildParams.Resource = Resource;
            BuildParams.Format = Resource->GetFormat();
            BuildParams.Access = Access;
            BuildParams.TextureViewAspect.Flags = TextureAspect.Flags;
            BuildParams.TextureViewAspect.FirstMipLevel = 0;
            BuildParams.TextureViewAspect.MipLevelCount = TextureAspect.MipLevelCount;
            BuildParams.TextureViewAspect.FirstArraySize = 0;
            BuildParams.TextureViewAspect.ArraySize = TextureAspect.ArraySize;
            Build(BuildParams);
        }
        else if (FlagHas(ResourceArchetype, E_RHIResourceArchetype::RTAS))
        {
            const auto& RTASAspect = Resource->GetRTASAspect();
            
            F_RHIRTASViewBuildParams BuildParams;
            BuildParams.Resource = Resource;
            BuildParams.Format = Resource->GetFormat();
            BuildParams.Access = Access;
            Build(BuildParams);
        }
        else
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid resource";
        }
    }

    TS<A_RHIContextChildProxy> A_RHIResourceView::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIResourceViewProxy>(ABYTEK_WTHIS());
    }
}
