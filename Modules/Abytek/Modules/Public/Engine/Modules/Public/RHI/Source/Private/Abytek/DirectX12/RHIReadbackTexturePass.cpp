#include "Abytek/DirectX12/RHIReadbackTexturePass.hpp"

#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/DirectX12/RHIReadbackTexturePassProxy.hpp"
#include "Abytek/DirectX12/RHISubmissionList.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHICopyReadbackTexturePass);
    void F_DirectX12RHICopyReadbackTexturePass::Build(const F_DirectX12RHICopyReadbackTexturePassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
        
        _ReadbackPass = BuildParams.ReadbackPass;
    }   
    void F_DirectX12RHICopyReadbackTexturePass::Release()
    {
        _ReadbackPass = {};
        
        A_DirectX12RHIPassExtension::Release();
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> F_DirectX12RHICopyReadbackTexturePass::CreateProxy()
    {
        return RACreateAndBuildShared<F_DirectX12RHICopyReadbackTexturePassProxy>(ABYTEK_WTHIS());
    }

    void F_DirectX12RHICopyReadbackTexturePass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        
        auto TransientReadbackBuffer = _ReadbackPass->GetTransientReadbackBufferRange().GetBuffer();
        auto Texture = _ReadbackPass->GetTexture();
        auto NumSubresources = _ReadbackPass->GetNumSubresources();
        
        SubresourceBindingSet.push_back(
            F_DirectX12RHISubresourceBinding::MakeCore(
                F_DirectX12RHISubresourceReference::Make(
                    TransientReadbackBuffer.Weak(),
                    0
                ),
                F_RHIResourceAccess::MakeCopyDest()
            )
        );
        for (U32 SubresourceIndex = 0; SubresourceIndex < NumSubresources; ++SubresourceIndex)
        {
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        Texture.Weak(),
                        SubresourceIndex
                    ),
                    F_RHIResourceAccess::MakeCopySrc()
                )
            );
        }
    }

    void F_DirectX12RHIReadbackTexturePass::Build(const F_RHIReadbackTexturePassBuildParams& BuildParams)
    {
        A_RHIReadbackTexturePass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
        
        auto Arena = GetProcess()->GetArena();
        
        auto Context = GetContext();
        auto Format = BuildParams.Texture->GetFormat();
        const auto& TextureAspect = BuildParams.Texture->GetTextureAspect();
        
        auto D3D12Device = Context->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        auto D3D12ResourceDesc = BuildParams.Texture.FastCast<F_DirectX12RHIResource>()->GetD3D12ResourceDesc();
        
        _NumSubresources = RHICalculateNumSubImages(Format, TextureAspect.MipLevelCount, TextureAspect.ArraySize);
        _SubImages_ReadbackFootprint.resize(_NumSubresources);
        _SubImages_NumRow.resize(_NumSubresources);
        
        D3D12Device->GetCopyableFootprints(
            &D3D12ResourceDesc,
            0,
            _NumSubresources,
            0,
            _SubImages_ReadbackFootprint.data(),
            _SubImages_NumRow.data(),
            nullptr,
            nullptr
        );
        
        U64* SubImages_RowPitchInBytes = Arena->AllocateData<U64>(_NumSubresources);
        for (U32 SubresourceIndex = 0; SubresourceIndex < _NumSubresources; ++SubresourceIndex)
        {
            const auto& SubImageFootprint = _SubImages_ReadbackFootprint[SubresourceIndex];
            SubImages_RowPitchInBytes[SubresourceIndex] = SubImageFootprint.Footprint.RowPitch;
        }
        
        auto ImagePtr = Arena->New_ReleaseOnReset<F_RHIImage>(
            true,
            Format, 
            TextureAspect, 
            TF_Span<const U64>(SubImages_RowPitchInBytes, SubImages_RowPitchInBytes + _NumSubresources)
        );
        _ImageView = *ImagePtr;
#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
        for (U32 SubresourceIndex = 0; SubresourceIndex < _NumSubresources; ++SubresourceIndex)
        {
            const auto& SubImageMemoryStructure = _ImageView.MemoryStructure.SubImages[SubresourceIndex];
            const auto& SubImageFootprint = _SubImages_ReadbackFootprint[SubresourceIndex];
            ABYTEK_ENGINE_RHI_ASSERT(SubImageFootprint.Offset == SubImageMemoryStructure.OffsetInBytes) << "Internal RHI image alignment bug on readback texture pass";
        }
#endif
        
        U64 ReadbackSizeInBytes = 0;
        for (U32 SubresourceIndex = 0; SubresourceIndex < _NumSubresources; ++SubresourceIndex)
        {
            const auto& SubImageFootprint = _SubImages_ReadbackFootprint[SubresourceIndex];
            ReadbackSizeInBytes = Max<U64>(
                ReadbackSizeInBytes, 
                SubImageFootprint.Offset + 
                SubImageFootprint.Footprint.RowPitch 
                * SubImageFootprint.Footprint.Height
                * SubImageFootprint.Footprint.Depth
            );
        }
        
        _TransientReadbackBufferRange = Context->GetTransientReadbackBufferManager()->Allocate(
              D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT - 1
              + ReadbackSizeInBytes
        );
        _TransientReadbackBufferRange.BeginOffsetInBytes = AlignAddress(
            _TransientReadbackBufferRange.BeginOffsetInBytes,
            D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT
        ); 
        _TransientReadbackBufferRange.EndOffsetInBytes = _TransientReadbackBufferRange.BeginOffsetInBytes + ReadbackSizeInBytes;
        for (U32 SubresourceIndex = 0; SubresourceIndex < _NumSubresources; ++SubresourceIndex)
        {
            auto& SubImageFootprint = _SubImages_ReadbackFootprint[SubresourceIndex];
            SubImageFootprint.Offset += _TransientReadbackBufferRange.BeginOffsetInBytes;
        }
        
        F_DirectX12RHICopyReadbackTexturePassBuildParams CopyPassBuildParams;
        CopyPassBuildParams.Context = Context;
        CopyPassBuildParams.ReadbackPass = ABYTEK_WTHIS();
        _CopyPass = RACreateAndBuildShared<F_DirectX12RHICopyReadbackTexturePass>(CopyPassBuildParams);
    }   
    void F_DirectX12RHIReadbackTexturePass::Release()
    {
        _CopyPass = {};
        _SubImages_NumRow = {};
        _SubImages_ReadbackFootprint = {};
        _NumSubresources = 0;
        _ImageView = {};
        _TransientReadbackBufferRange = {};
        
        A_DirectX12RHIPassExtension::Release();
        A_RHIReadbackTexturePass::Release();
    }

    void F_DirectX12RHIReadbackTexturePass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        SubresourceBindingSet.push_back(
            F_DirectX12RHISubresourceBinding::MakeCore(
                F_DirectX12RHISubresourceReference::Make(
                    _TransientReadbackBufferRange.GetBuffer().Weak(),
                    0
                ),
                F_RHIResourceAccess::MakeReadback()
            )
        );
    }

    E_DirectX12RHIPassBatchType F_DirectX12RHIReadbackTexturePass::GetPassBatchType()
    {
        return E_DirectX12RHIPassBatchType::CPU_SYNC;
    }

    void F_DirectX12RHIReadbackTexturePass::OnAddItemsBefore(I_RHISubmissionItemContainer& Container)
    {
        if (_CopyPass)
        {
            Container.AddSubmissionItem(_CopyPass);
        }
    }

    B8 F_DirectX12RHIReadbackTexturePass::CanDetachCopyPass()
    {
        return static_cast<B8>(_CopyPass);
    }
    void F_DirectX12RHIReadbackTexturePass::DetachCopyPass(I_RHISubmissionItemContainer& SubmissionItemContainer)
    {
        ABYTEK_ENGINE_RHI_ASSERT(_CopyPass) << "Cannot detach copy pass";
        SubmissionItemContainer.AddSubmissionItem(_CopyPass);
        _CopyPass = {};
    }

#ifdef ABYTEK_DEBUG_INFO
    void F_DirectX12RHIReadbackTexturePass::SetDebugName(const F_DebugName& Value) noexcept
    {
        A_RHIReadbackTexturePass::SetDebugName(Value);
        if (_CopyPass)
        {
            _CopyPass->SetDebugName(F_Name(ToText(*GetDebugName()) + ABYTEK_TEXT(".CopyPass")));
        }
    }
#endif
}
#endif