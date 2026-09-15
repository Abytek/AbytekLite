#include "Abytek/DirectX12/RHIResource.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIProcess.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/DirectX12/RHIFormat.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    namespace Internal::DirectX12RHIResource
    {
        void AddD3D12StatesMinimal(const F_RHIResourceAccess& Access, D3D12_RESOURCE_STATES& Result)
        {        
            if (FlagHas(Access.GPU, E_RHIResourceGPUAccess::GENERIC_READ_))
            {
                Result |= D3D12_RESOURCE_STATE_GENERIC_READ;
            }
            if (FlagHas(Access.GPU, E_RHIResourceGPUAccess::RTAS))
            {
                Result |= D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
            }
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::UAV))
            {
                Result |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
            }
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::RTV))
            {
                Result |= D3D12_RESOURCE_STATE_RENDER_TARGET;
            }
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::DSV))
            {
                Result |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
            }
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::CBV))
            {
                Result |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
            }
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::VERTEX_BUFFER))
            {
                Result |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
            }
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::INDEX_BUFFER))
            {
                Result |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
            }
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::COPY_SOURCE))
            {
                Result |= D3D12_RESOURCE_STATE_COPY_SOURCE;
            }
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::COPY_DEST))
            {
                Result |= D3D12_RESOURCE_STATE_COPY_DEST;
            }
        }

        D3D12_RESOURCE_STATES RHIResourceAccessToD3D12ResourceStates_Simple(
            const F_RHIResourceAccess& Access
        )
        {
            D3D12_RESOURCE_STATES Result = (D3D12_RESOURCE_STATES)0;
            Internal::DirectX12RHIResource::AddD3D12StatesMinimal(Access, Result);
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::SRV))
            {
                Result |= D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE;
            }
            return Result;
        }
        D3D12_RESOURCE_STATES RHIResourceAccessToD3D12ResourceStates_Detailed(
            const F_RHIResourceAccess& Access
        )
        {
            D3D12_RESOURCE_STATES Result = (D3D12_RESOURCE_STATES)0;
            Internal::DirectX12RHIResource::AddD3D12StatesMinimal(Access, Result);
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::SRV))
            {
                if (FlagHas(Access.ShaderFrequency, E_RHIShaderFrequency::PIXEL))
                {
                    Result |= D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
                }
                if (FlagRemove(Access.ShaderFrequency, E_RHIShaderFrequency::PIXEL) != E_RHIShaderFrequency::NONE)
                {
                    Result |= D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
                }
            }
            return Result;
        }

        D3D12_RESOURCE_FLAGS RHIResourceAccessToD3D12ResourceFlags(const F_RHIResourceAccess& Access)
        {
            D3D12_RESOURCE_FLAGS Result = D3D12_RESOURCE_FLAG_NONE;
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::RTV))
            {
                Result |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            }
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::DSV))
            {
                Result |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
            }
            if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::UAV))
            {
                Result |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            }
            if (
                FlagHasAny(Result, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
                && !FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::SRV)
            )
            {
                Result |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
            }
            return Result;
        }
    }

    F_DirectX12RHIResourceUse& F_DirectX12RHIResourceUseReference::Get() const noexcept
    {
        return PassExtension->GetProcessData_PassExtension()->ResourceUseSet[Index];
    }
    F_DirectX12RHIResourceUse* F_DirectX12RHIResourceUseReference::operator->() const noexcept
    {
        return &Get();
    }
    F_DirectX12RHIResourceUse& F_DirectX12RHIResourceUseReference::operator*() const noexcept
    {
        return Get();
    }

    F_DirectX12RHIResourcePassTracking& F_DirectX12RHIResourcePassTrackingReference::Get() const noexcept
    {
        return Resource.FastCast<F_DirectX12RHIResource>()->GetCompileData()->PassTrackings[Index];
    }
    F_DirectX12RHIResourcePassTracking* F_DirectX12RHIResourcePassTrackingReference::operator->() const noexcept
    {
        return &Get();
    }
    F_DirectX12RHIResourcePassTracking& F_DirectX12RHIResourcePassTrackingReference::operator*() const noexcept
    {
        return Get();
    }
    
    F_Text F_DirectX12RHISubresourceReference::GetInfoText(U32 NumTabs) const
    {
        if (!IsValid())
        {
            return ABYTEK_TEXT("Invalid subresource reference");
        }
        
        F_Text Result;
        
        F_Text Tabs;
        for (U32 Index = 0; Index < NumTabs; ++Index)
        {
            Tabs += ABYTEK_TEXT("\t");
        }
        
        Result += Tabs + ABYTEK_TEXT("Resource: ") + ToText(*Resource->GetDebugName());
        Result += ToText(F_NewLine {});
        Result += Tabs + ABYTEK_TEXT("Referenced subresource: ");
        Result += ToText(F_NewLine {});
        
        Result += Get().GetInfoText(NumTabs + 1);
        
        return ABYTEK_MOVE(Result);
    }
    
    F_DirectX12RHISubresource& F_DirectX12RHISubresourceReference::Get() const noexcept
    {
        return Resource.FastCast<F_DirectX12RHIResource>()->InjectSubresourceSet()[Index];
    }
    F_DirectX12RHISubresource* F_DirectX12RHISubresourceReference::operator->() const noexcept
    {
        return &Get();
    }
    F_DirectX12RHISubresource& F_DirectX12RHISubresourceReference::operator*() const noexcept
    {
        return Get();
    }

    F_DirectX12RHISubresourceBinding& F_DirectX12RHISubresourceBindingReference::Get() const noexcept
    {
        return PassExtension->GetProcessData_PassExtension()->SubresourceBindingSet[Index];
    }
    F_DirectX12RHISubresourceBinding* F_DirectX12RHISubresourceBindingReference::operator->() const noexcept
    {
        return &Get();
    }
    F_DirectX12RHISubresourceBinding& F_DirectX12RHISubresourceBindingReference::operator*() const noexcept
    {
        return Get();
    }
    
    F_DirectX12RHISubresourcePassTracking& F_DirectX12RHISubresourcePassTrackingReference::Get() const noexcept
    {
        return SubresourceReference->GetCompileData()->PassTrackings[Index];
    }
    F_DirectX12RHISubresourcePassTracking* F_DirectX12RHISubresourcePassTrackingReference::operator -> () const noexcept
    {
        return &Get();
    }
    F_DirectX12RHISubresourcePassTracking& F_DirectX12RHISubresourcePassTrackingReference::operator * () const noexcept
    {
        return Get();
    }

    F_Text F_DirectX12RHISubresource::GetInfoText(U32 NumTabs) const
    {
        F_Text Result;
        
        F_Text Tabs;
        for (U32 Index = 0; Index < NumTabs; ++Index)
        {
            Tabs += ABYTEK_TEXT("\t");
        }
        
        Result += Tabs + ABYTEK_TEXT("Type: ");
        switch (Type)
        {
        case E_RHISubresourceType::BUFFER:
            Result += ABYTEK_TEXT("Buffer");
            break;
        case E_RHISubresourceType::TEXTURE:
            Result += ABYTEK_TEXT("Texture");
            break;
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown subresource type";
            break;
        }
        Result += ToText(F_NewLine {});
        
        Result += Tabs + ABYTEK_TEXT("Index: ") + ToText(Index);
        Result += ToText(F_NewLine {});
        
        switch (Type)
        {
        case E_RHISubresourceType::BUFFER:
            break;
        case E_RHISubresourceType::TEXTURE:
            Result += Tabs + ABYTEK_TEXT("Texture.ArraySlice: ") + ToText(Texture.ArraySlice);
            Result += ToText(F_NewLine {});
            Result += Tabs + ABYTEK_TEXT("Texture.MipIndex: ") + ToText(Texture.MipIndex);
            Result += ToText(F_NewLine {});
            Result += Tabs + ABYTEK_TEXT("Texture.PlaneIndex: ") + ToText(Texture.PlaneIndex);
            break;
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown subresource type";
            break;
        }
        
        return ABYTEK_MOVE(Result);
    }
    
    D3D12_RESOURCE_BARRIER F_DirectX12RHIResourceBarrierProxy::ConvertToD3D12ResourceBarrier() const noexcept
    {
        D3D12_RESOURCE_BARRIER Result;
        Result.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        switch (Type)
        {
        case E_DirectX12RHIResourceBarrierProxyType::TRANSITION:
            Result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            Result.Transition.pResource = Transition.ResourceProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource().Get();
            Result.Transition.Subresource = Transition.SubresourceIndex;
            Result.Transition.StateBefore = Transition.D3D12StateBefore;
            Result.Transition.StateAfter = Transition.D3D12StateAfter;
            break;
        case E_DirectX12RHIResourceBarrierProxyType::UAV:
            Result.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
            Result.UAV.pResource = UAV.ResourceProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource().Get();
            break;
        case E_DirectX12RHIResourceBarrierProxyType::ALIASING:
            Result.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
            Result.Aliasing.pResourceBefore = Aliasing.ResourceProxyBefore.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource().Get();
            Result.Aliasing.pResourceAfter = Aliasing.ResourceProxyAfter.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource().Get();
            break;
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid resource barrier proxy type";
            break;
        }
        return Result;
    }

    void H_DirectX12RHIResourceBarrierProxy::AddResourceBarrier(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& InD3D12CommandList,
        const F_DirectX12RHIResourceBarrierProxySet& BarrierProxies
    )
    {
        if (BarrierProxies.size() == 0)
        {
            return;
        }
        TF_SmallVector<D3D12_RESOURCE_BARRIER, 16> D3D12ResourceBarriers;
        D3D12ResourceBarriers.reserve(BarrierProxies.size());
        for (const auto& ResourceBarrierProxy : BarrierProxies)
        {
            D3D12ResourceBarriers.push_back(ResourceBarrierProxy.ConvertToD3D12ResourceBarrier());
        }
        InD3D12CommandList->ResourceBarrier(D3D12ResourceBarriers.size(), D3D12ResourceBarriers.data());
    }

    F_DirectX12RHISubresourceSet H_DirectX12RHISubresourceSet::MakeBuffer(
        const TW<A_RHIContext>& Context,
        E_RHIFormat Format,
        const F_RHIBufferAspect& BufferAspect,
        D3D12_RESOURCE_STATES InitialD3D12State
    )
    {
        F_DirectX12RHISubresourceSet Result;
        Result.reserve(1);

        F_DirectX12RHISubresource Subresource{};
        Subresource.Context = Context;
        Subresource.Type = E_RHISubresourceType::BUFFER;
        Subresource.Index = 0;
                
        Subresource.LastD3D12State = InitialD3D12State;

        Result.push_back(Subresource);

        return Result;
    }
    F_DirectX12RHISubresourceSet H_DirectX12RHISubresourceSet::MakeTexture(
        const TW<A_RHIContext>& Context,
        E_RHIFormat Format,
        const F_RHITextureAspect& TextureAspect,
        D3D12_RESOURCE_STATES InitialD3D12State
    )
    {
        F_DirectX12RHISubresourceSet Result;

        const U32 MipCount   = TextureAspect.MipLevelCount;
        const U32 ArraySize  = TextureAspect.ArraySize;

        Result.reserve(
            RHICalculateNumSubImages(
                Format,
                MipCount, 
                ArraySize
            )
        );

        U32 SubresourceIndex = 0;

        for (U32 PlaneIndex = 0; PlaneIndex < RHIFormatPlaneCount(Format); ++PlaneIndex)
        {
            for (U32 ArraySlice = 0; ArraySlice < ArraySize; ++ArraySlice)
            {
                for (U32 MipIndex = 0; MipIndex < MipCount; ++MipIndex)
                {
                    F_DirectX12RHISubresource Subresource{};
                    Subresource.Context = Context;
                    Subresource.Type = E_RHISubresourceType::TEXTURE;
                    Subresource.Index = SubresourceIndex++;
                
                    Subresource.LastD3D12State = InitialD3D12State;

                    Subresource.Texture.ArraySlice = ArraySlice;
                    Subresource.Texture.MipIndex   = MipIndex;
                    Subresource.Texture.PlaneIndex   = PlaneIndex;

                    Result.push_back(Subresource);
                }
            }
        }

        return Result;
    }

    D3D12_HEAP_TYPE H_DirectX12RHIResource::GetD3D12HeapType(const F_RHIResourceAccess& AccessCapabilities)
    {
        return DirectX12SharedAPIWrapper::Conversions::RHIResourceAccessToD3D12HeapType(AccessCapabilities);
    }
    D3D12_HEAP_PROPERTIES H_DirectX12RHIResource::GetD3D12HeapProperties(const F_RHIResourceAccess& AccessCapabilities)
    {
        return CD3DX12_HEAP_PROPERTIES(GetD3D12HeapType(AccessCapabilities));
    }
    D3D12_RESOURCE_STATES H_DirectX12RHIResource::GetInitialD3D12State(
        const F_RHIResourceAccess& AccessCapabilities,
        const TF_Optional<F_RHIResourceAccess>& InitialAccessOpt
    )
    {
        return Internal::DirectX12RHIResource::RHIResourceAccessToInitialD3D12ResourceStates_Simple(
            AccessCapabilities,
            InitialAccessOpt
        );;
    }
    D3D12_RESOURCE_DESC H_DirectX12RHIResource::GetD3D12ResourceDesc(
        const F_RHIResourceAccess& AccessCapabilities,
        E_RHIFormat Format,
        const F_RHIBufferAspect& BufferAspect
    )
    {
        D3D12_RESOURCE_DESC Result = {};
        Result.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        Result.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
        Result.Width = AlignSize_PO2(BufferAspect.SizeInBytes, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
        Result.Height = 1;
        Result.DepthOrArraySize = 1;
        Result.MipLevels = 1;
        Result.Format = DXGI_FORMAT_UNKNOWN;
        Result.SampleDesc.Count = 1;
        Result.SampleDesc.Quality = 0;
        Result.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        Result.Flags = Internal::DirectX12RHIResource::RHIResourceAccessToD3D12ResourceFlags(AccessCapabilities);
        return Result;
    }
    D3D12_RESOURCE_DESC H_DirectX12RHIResource::GetD3D12ResourceDesc(
        const F_RHIResourceAccess& AccessCapabilities,
        E_RHIFormat Format,
        const F_RHITextureAspect& TextureAspect
    )
    {
        D3D12_RESOURCE_DESC Result = {};
        static const D3D12_RESOURCE_DIMENSION D3D12Dimensions[]
        {
            D3D12_RESOURCE_DIMENSION_TEXTURE1D,
            D3D12_RESOURCE_DIMENSION_TEXTURE2D,
            D3D12_RESOURCE_DIMENSION_TEXTURE3D
        };
        Result.Dimension = D3D12Dimensions[TextureAspect.DimensionCount - 1];
        Result.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
        Result.Width = TextureAspect.Width;
        Result.Height = TextureAspect.Height;
        Result.DepthOrArraySize = ((TextureAspect.DimensionCount == 3) ? TextureAspect.Depth : TextureAspect.ArraySize);
        Result.MipLevels = TextureAspect.MipLevelCount;
        Result.Format = RHIFormatToD3DFormat(Format);
        Result.SampleDesc.Count = TextureAspect.SampleDesc.Count;
        Result.SampleDesc.Quality = TextureAspect.SampleDesc.Quality;
        Result.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        Result.Flags = Internal::DirectX12RHIResource::RHIResourceAccessToD3D12ResourceFlags(AccessCapabilities);
        return Result;
    }
    TF_Optional<D3D12_CLEAR_VALUE> H_DirectX12RHIResource::GetD3D12ClearValue(
        const F_RHIResourceAccess& AccessCapabilities,
        E_RHIFormat Format,
        const F_RHITextureAspect& TextureAspect
    )
    {
        if (
            !(
                FlagHas(AccessCapabilities.GPU, E_RHIResourceGPUAccess::RTV)    
                || FlagHas(AccessCapabilities.GPU, E_RHIResourceGPUAccess::DSV)    
            )    
        )
        {
            return {};
        }
        
        E_RHIFormat ActualFormat = TextureAspect.ClearValue.Format;
        if (ActualFormat == E_RHIFormat::NONE)
        {
            ActualFormat = Format;
        }
        if (
            (ActualFormat != E_RHIFormat::NONE)
            && !RHIFormatIsTypeless(ActualFormat)
        )
        {
            D3D12_CLEAR_VALUE ClearValue;
            ClearValue.Format = RHIFormatToD3DFormat(ActualFormat);
            if (RHIFormatHasColor(ActualFormat))
            {
                ClearValue.Color[0] = TextureAspect.ClearValue.Color.X;
                ClearValue.Color[1] = TextureAspect.ClearValue.Color.Y;
                ClearValue.Color[2] = TextureAspect.ClearValue.Color.Z;
                ClearValue.Color[3] = TextureAspect.ClearValue.Color.W;
            }
            if (RHIFormatHasDepth(ActualFormat))
            {
                ClearValue.DepthStencil.Depth = TextureAspect.ClearValue.Depth;
            }
            if (RHIFormatHasStencil(ActualFormat))
            {
                ClearValue.DepthStencil.Stencil = TextureAspect.ClearValue.Stencil;
            }
            return ClearValue;
        }
        return {};
    }

    void F_DirectX12RHIResource::Release()
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        if (_Placement)
        {
            GetContext().FastCast<F_DirectX12RHIContext>()->Queues.ResourcePlacementsToDeallocate.Push(
                _Placement
            );
            _Placement = {};
        }
        _SubresourceSet = {};
        A_RHIResource::Release();
    }

    void F_DirectX12RHIResource::Build(const F_RHIBufferBuildParams& BuildParams)
    {
        Build(BuildParams, BuildParams.BufferAspect);
        
        if (FlagHas(BuildParams.AdditionalFlags, E_RHIResourceAdditionalFlag::AUTO_PLACED))
        {
            D3D12CreateAutoPlaced();
        }
        else
        {
            D3D12CreateCommitted();
        }
        if (BuildParams.BufferDataView)
        {
            D3D12Upload(*BuildParams.BufferDataView);
        }
    }
    void F_DirectX12RHIResource::Build(const F_RHITextureBuildParams& BuildParams)
    {
        Build(BuildParams, BuildParams.TextureAspect);
        
        if (FlagHas(BuildParams.AdditionalFlags, E_RHIResourceAdditionalFlag::AUTO_PLACED))
        {
            D3D12CreateAutoPlaced();
        }
        else
        {
            D3D12CreateCommitted();
        }
        if (BuildParams.TextureDataView)
        {
            D3D12Upload(*BuildParams.TextureDataView);
        }
    }
    void F_DirectX12RHIResource::Build(const F_RHIRTASBuildParams& BuildParams)
    {
        Build(BuildParams, BuildParams.RTASAspect);
        
        if (FlagHas(BuildParams.AdditionalFlags, E_RHIResourceAdditionalFlag::AUTO_PLACED))
        {
            D3D12CreateAutoPlaced();
        }
        else
        {
            D3D12CreateCommitted();
        }
        if (BuildParams.RTASDataView)
        {
            D3D12Upload(*BuildParams.RTASDataView);
        }
    }

    void F_DirectX12RHIResource::Build(
        const F_RHIResourceBuildParamsBase& BuildParamsBase,
        const F_RHIBufferAspect& BufferAspect
    )
    {
        A_RHIResource::Build(BuildParamsBase, BufferAspect);
        GetProxy().FastCast<F_DirectX12RHIResourceProxy>()->LateBuildBuffer_DirectX12();
        D3D12EarlySetupBuffer();
    }
    void F_DirectX12RHIResource::Build(
        const F_RHIResourceBuildParamsBase& BuildParamsBase,
        const F_RHITextureAspect& TextureAspect
    )
    {
        A_RHIResource::Build(BuildParamsBase, TextureAspect);
        GetProxy().FastCast<F_DirectX12RHIResourceProxy>()->LateBuildTexture_DirectX12();
        D3D12EarlySetupTexture();
    }
    void F_DirectX12RHIResource::Build(
        const F_RHIResourceBuildParamsBase& BuildParamsBase,
        const F_RHIRTASAspect& RTASAspect
    )
    {
        A_RHIResource::Build(BuildParamsBase, RTASAspect);
        GetProxy().FastCast<F_DirectX12RHIResourceProxy>()->LateBuildRTAS_DirectX12();
        D3D12EarlySetupRTAS();
    }

    void F_DirectX12RHIResource::BuildExternal(
        const F_RHIResourceBuildParamsBase& BuildParamsBase, 
        const F_RHIBufferAspect& BufferAspect, 
        const Microsoft::WRL::ComPtr<ID3D12Resource>& ExternalD3D12Resource
    )
    {
        Build(BuildParamsBase, BufferAspect);
        D3D12AssignExternal(ExternalD3D12Resource);
    }
    void F_DirectX12RHIResource::BuildExternal(
        const F_RHIResourceBuildParamsBase& BuildParamsBase, 
        const F_RHITextureAspect& TextureAspect, 
        const Microsoft::WRL::ComPtr<ID3D12Resource>& ExternalD3D12Resource
    )
    {
        Build(BuildParamsBase, TextureAspect);
        D3D12AssignExternal(ExternalD3D12Resource);
    }
    void F_DirectX12RHIResource::BuildExternal(
        const F_RHIResourceBuildParamsBase& BuildParamsBase, 
        const F_RHIRTASAspect& RTASAspect, 
        const Microsoft::WRL::ComPtr<ID3D12Resource>& ExternalD3D12Resource
    )
    {
        Build(BuildParamsBase, RTASAspect);
        D3D12AssignExternal(ExternalD3D12Resource);
    }

    void F_DirectX12RHIResource::BuildWithoutD3D12Resource(
        const F_RHIResourceBuildParamsBase& BuildParamsBase,
        const F_RHIBufferAspect& BufferAspect
    )
    {
        Build(BuildParamsBase, BufferAspect);
    }
    void F_DirectX12RHIResource::BuildWithoutD3D12Resource(
        const F_RHIResourceBuildParamsBase& BuildParamsBase,
        const F_RHITextureAspect& TextureAspect
    )
    {
        Build(BuildParamsBase, TextureAspect);
    }
    void F_DirectX12RHIResource::BuildWithoutD3D12Resource(
        const F_RHIResourceBuildParamsBase& BuildParamsBase,
        const F_RHIRTASAspect& RTASAspect
    )
    {
        Build(BuildParamsBase, RTASAspect);
    }

    void F_DirectX12RHIResource::Build(const F_RHIResourceBuildParamsBase& BuildParams)
    {
        A_RHIResource::Build(BuildParams);
        GetProxy().FastCast<F_DirectX12RHIResourceProxy>()->LateBuildResource_DirectX12();
        
        auto AdditionalFlags = GetAdditionalFlags();
        if (FlagHas(AdditionalFlags, E_RHIResourceAdditionalFlag::RELEASE_AFTER_SUBMIT))
        {
            D3D12ReleaseAfterSubmit();
        }
        if (FlagHas(AdditionalFlags, E_RHIResourceAdditionalFlag::STATIC))
        {
            D3D12EnableStaticAfterSubmit();
        }
    }

    void F_DirectX12RHIResource::ReleaseAfterSubmit()
    {
        A_RHIResource::ReleaseAfterSubmit();
        D3D12ReleaseAfterSubmit();
    }

    void F_DirectX12RHIResource::EnableStaticAfterSubmit(const F_RHIResourceAccess& StaticAccess)
    {
        A_RHIResource::EnableStaticAfterSubmit(StaticAccess);
        D3D12EnableStaticAfterSubmit();
    }
    void F_DirectX12RHIResource::DisableStatic()
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
        if (FlagHas(GetAdditionalFlags(), E_RHIResourceAdditionalFlag::STATIC))
        {
            auto CompileData_Resource = GetCompileData();
            ABYTEK_ENGINE_RHI_ASSERT(CompileData_Resource->IsUsedAsStatic.test(boost::memory_order_acquire) == false) 
                << "Cannot disable static mode on this resource (" << *GetDebugName() << ") because it was already used by other passes as static";
        }
#endif
        A_RHIResource::DisableStatic();
    }

    void F_DirectX12RHIResource::D3D12EarlySetupBuffer()
    {
        _SubresourceSet = H_DirectX12RHISubresourceSet::MakeBuffer(
            GetContext(),
            GetFormat(),
            GetBufferAspect(),
            GetInitialD3D12State()
        );
    }
    void F_DirectX12RHIResource::D3D12EarlySetupTexture()
    {
        _SubresourceSet = H_DirectX12RHISubresourceSet::MakeTexture(
            GetContext(),
            GetFormat(),
            GetTextureAspect(),
            GetInitialD3D12State()
        );
    }
    void F_DirectX12RHIResource::D3D12EarlySetupRTAS()
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "RTAS was not supported";
    }

    void F_DirectX12RHIResource::D3D12Upload(const F_RHIBufferDataView& DataView)
    {
        DirectX12RHIProcessQueries::Compile::F_UploadBuffer Query;
        Query.Resource = ABYTEK_STHIS();
        Query.BufferDataView = DataView;
        
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()
        ->Queues.Compile.UploadBuffer.Push(Query);
    }
    void F_DirectX12RHIResource::D3D12Upload(const F_RHITextureDataView& DataView)
    {
        DirectX12RHIProcessQueries::Compile::F_UploadTexture Query;
        Query.Resource = ABYTEK_STHIS();
        Query.TextureDataView = DataView;
        
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()
        ->Queues.Compile.UploadTexture.Push(Query);
    }
    void F_DirectX12RHIResource::D3D12Upload(const F_RHIRTASDataView& DataView)
    {
        DirectX12RHIProcessQueries::Compile::F_UploadRTAS Query;
        Query.Resource = ABYTEK_STHIS();
        Query.RTASDataView = DataView;
        
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()
        ->Queues.Compile.UploadRTAS.Push(Query);
    }

    void F_DirectX12RHIResource::D3D12AssignExternal(const Microsoft::WRL::ComPtr<ID3D12Resource>& ExternalD3D12Resource)
    {
        DirectX12RHIProcessQueries::Execution::F_AssignExternalD3D12Resource Query;
        Query.ResourceProxy = GetProxy().FastCast<A_RHIResourceProxy>();
        Query.D3D12Resource = ExternalD3D12Resource;
        
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()
        ->Queues.Execution.AssignExternalD3D12Resource.Push(Query);
    }
    void F_DirectX12RHIResource::D3D12CreateCommitted()
    {
        DirectX12RHIProcessQueries::Execution::F_CreateCommittedResource Query;
        Query.ResourceProxy = GetProxy().FastCast<A_RHIResourceProxy>();
        Query.D3D12HeapProperties = GetD3D12HeapProperties();
        Query.InitialD3D12ResourceState = GetInitialD3D12State();
        Query.D3D12ResourceDesc = GetD3D12ResourceDesc();
        Query.D3D12ClearValue = GetD3D12ClearValue();
        
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()
        ->Queues.Execution.CreateCommittedResource.Push(Query);
    }
    void F_DirectX12RHIResource::D3D12CreateAutoPlaced()
    {
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()
        ->Queues.Compile.AutoPlacedResourcesToAllocate.Push(ABYTEK_STHIS());
    }
    void F_DirectX12RHIResource::D3D12CreatePlaced()
    {
        DirectX12RHIProcessQueries::Execution::F_CreatePlacedResource Query;
        Query.ResourceProxy = GetProxy().FastCast<A_RHIResourceProxy>();
        Query.InitialD3D12ResourceState = GetInitialD3D12State();
        Query.D3D12ResourceDesc = GetD3D12ResourceDesc();
        Query.D3D12ClearValue = GetD3D12ClearValue();
        Query.ResourcePlacementProxy = F_DirectX12RHIResourcePlacementProxy::From(GetPlacement());
        
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()
        ->Queues.Execution.CreatePlacedResource.Push(Query);
    }

    void F_DirectX12RHIResource::D3D12ReleaseAfterSubmit()
    {
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()
        ->Queues.Compile.AutoPlacedResourcesToAllocate.Push(ABYTEK_STHIS());
    }
    void F_DirectX12RHIResource::D3D12EnableStaticAfterSubmit()
    {
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()
        ->Queues.Compile.ResourceStaticTransition.Push({ ABYTEK_STHIS() });
    }

    D3D12_HEAP_TYPE F_DirectX12RHIResource::GetD3D12HeapType()
    {
        return H_DirectX12RHIResource::GetD3D12HeapType(GetAccessCapabilities());
    }
    D3D12_HEAP_PROPERTIES F_DirectX12RHIResource::GetD3D12HeapProperties()
    {
        return H_DirectX12RHIResource::GetD3D12HeapProperties(GetAccessCapabilities());
    }
    D3D12_RESOURCE_STATES F_DirectX12RHIResource::GetInitialD3D12State()
    {
        return H_DirectX12RHIResource::GetInitialD3D12State(
            GetAccessCapabilities(),
            GetInitialAccess()
        );;
    }
    D3D12_RESOURCE_DESC F_DirectX12RHIResource::GetD3D12ResourceDesc()
    {
        D3D12_RESOURCE_DESC Result = {};
        
        auto Archetype = GetArchetype();
        auto AccessCapabilities = GetAccessCapabilities();
        
        if (FlagHas(Archetype, E_RHIResourceArchetype::RTAS))
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "RTAS was not supported";
        }
        else if (FlagHas(Archetype, E_RHIResourceArchetype::TEXTURE))
        {
            const auto& TextureAspect = GetTextureAspect();
            auto Format = GetFormat();
            return H_DirectX12RHIResource::GetD3D12ResourceDesc(
                AccessCapabilities,
                Format,
                TextureAspect
            );
        }
        else if (FlagHas(Archetype, E_RHIResourceArchetype::BUFFER))
        {
            const auto& BufferAspect = GetBufferAspect();
            auto Format = GetFormat();
            return H_DirectX12RHIResource::GetD3D12ResourceDesc(
                AccessCapabilities,
                Format,
                BufferAspect
            );
        }
        else
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid archetype";
        }
        return Result;
    }
    TF_Optional<D3D12_CLEAR_VALUE> F_DirectX12RHIResource::GetD3D12ClearValue()
    {
        if (!FlagHas(GetArchetype(), E_RHIResourceArchetype::TEXTURE))
        {
            return {};
        }
        return H_DirectX12RHIResource::GetD3D12ClearValue(
            GetAccessCapabilities(),
            GetFormat(),
            GetTextureAspect()
        );
    }
    D3D12_RESOURCE_ALLOCATION_INFO F_DirectX12RHIResource::GetD3D12ResourceAllocationInfo()
    {
        auto D3D12ResourceDesc = GetD3D12ResourceDesc();
        
        auto D3D12Device = GetContext()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        return D3D12Device->GetResourceAllocationInfo(
            1, 
            1, 
            &D3D12ResourceDesc
        );
    }
}
#endif