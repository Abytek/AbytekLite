#include "Abytek/DirectX12/RHIUploadTexturePass.hpp"
#include "Abytek/DirectX12/RHIUploadTexturePassProxy.hpp"
#include "Abytek/DirectX12/RHICopyBufferPass.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIUploadTexturePass::Build(const F_RHIUploadTexturePassBuildParams& BuildParams)
    {
        A_RHIUploadTexturePass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
        
        auto Context = GetContext();
        auto Format = BuildParams.Texture->GetFormat();
        const auto& TextureAspect = BuildParams.Texture->GetTextureAspect();
        
        auto D3D12Device = Context->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        auto D3D12ResourceDesc = BuildParams.Texture.FastCast<F_DirectX12RHIResource>()->GetD3D12ResourceDesc();
    
        _NumSubresources = RHICalculateNumSubImages(Format, TextureAspect.MipLevelCount, TextureAspect.ArraySize);
        ABYTEK_ENGINE_RHI_ASSERT(_NumSubresources == BuildParams.TextureDataView.Setting.SubImages.size());
        _SubImages_Bytes.resize(_NumSubresources);
        _SubImages_UploadFootprint.resize(_NumSubresources);
        
        D3D12Device->GetCopyableFootprints(
            &D3D12ResourceDesc,
            0,
            _NumSubresources,
            0,
            _SubImages_UploadFootprint.data(),
            nullptr,
            nullptr,
            nullptr
        );
        
        U64 UploadSizeInBytes = 0;
        for (U32 SubresourceIndex = 0; SubresourceIndex < _NumSubresources; ++SubresourceIndex)
        {
            const auto& SubImageFootprint = _SubImages_UploadFootprint[SubresourceIndex];
            UploadSizeInBytes = Max<U64>(
                UploadSizeInBytes, 
                SubImageFootprint.Offset 
                + SubImageFootprint.Footprint.RowPitch
                * SubImageFootprint.Footprint.Height
                * SubImageFootprint.Footprint.Depth
            );
        }
        
        auto Arena = GetProcess()->GetArena();
        for (U32 SubresourceIndex = 0; SubresourceIndex < _NumSubresources; ++SubresourceIndex)
        {
            const auto& SubImageSetting = BuildParams.TextureDataView.Setting.SubImages[SubresourceIndex];
            const auto& SubImageMemoryStructure = BuildParams.TextureDataView.MemoryStructure.SubImages[SubresourceIndex];
            const auto& SubImageFootprint = _SubImages_UploadFootprint[SubresourceIndex];
            Sz SubImageSizeInBytes = (
                SubImageFootprint.Footprint.RowPitch
                * SubImageFootprint.Footprint.Height
                * SubImageFootprint.Footprint.Depth    
            );
            if (SubImageMemoryStructure.RowPitchInBytes == SubImageFootprint.Footprint.RowPitch)
            {
                _SubImages_Bytes[SubresourceIndex] = TF_Span<const U8>(
                    BuildParams.TextureDataView.Payload.Bytes.begin() + SubImageMemoryStructure.OffsetInBytes,    
                    BuildParams.TextureDataView.Payload.Bytes.begin() + SubImageMemoryStructure.OffsetInBytes + SubImageMemoryStructure.SizeInBytes
                );
            }
            else
            {
                U8* DataPtr = Arena->AllocateData<U8>(SubImageSizeInBytes);
                for (U32 D = 0; D < SubImageSetting.Depth; ++D)
                {
                    for (U32 H = 0; H < SubImageSetting.Height; ++H)
                    {
                        U8* DstRowPtr = (
                            DataPtr 
                            + static_cast<U64>(H) * SubImageFootprint.Footprint.RowPitch
                            + static_cast<U64>(D) * SubImageFootprint.Footprint.RowPitch * SubImageSetting.Height
                        );
                        const U8* SrcRowPtr = (
                            BuildParams.TextureDataView.Payload.Bytes.begin() 
                            + SubImageMemoryStructure.OffsetInBytes 
                            + static_cast<U64>(H) * SubImageMemoryStructure.RowPitchInBytes
                            + static_cast<U64>(D) * SubImageMemoryStructure.RowPitchInBytes * SubImageSetting.Height
                        );
                        memcpy(
                            DstRowPtr,
                            SrcRowPtr,
                            SubImageMemoryStructure.RowPitchInBytes
                        );
                    }
                }
                _SubImages_Bytes[SubresourceIndex] = TF_Span<const U8>(
                    DataPtr,    
                    DataPtr + SubImageSizeInBytes
                );
            }
        }
        
        _TransientUploadBufferRange = Context->GetTransientUploadBufferManager_V2()->Allocate(
              D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT - 1
              + UploadSizeInBytes
        );
        _TransientUploadBufferRange.BeginOffsetInBytes = AlignAddress(
            _TransientUploadBufferRange.BeginOffsetInBytes,
            D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT
        ); 
        _TransientUploadBufferRange.EndOffsetInBytes = _TransientUploadBufferRange.BeginOffsetInBytes + UploadSizeInBytes;
        for (U32 SubresourceIndex = 0; SubresourceIndex < _NumSubresources; ++SubresourceIndex)
        {
            auto& SubImageFootprint = _SubImages_UploadFootprint[SubresourceIndex];
            SubImageFootprint.Offset += _TransientUploadBufferRange.BeginOffsetInBytes;
        }
        
        for (U32 SubresourceIndex = 0; SubresourceIndex < _NumSubresources; ++SubresourceIndex)
        {
            const auto& SubImage_Bytes = _SubImages_Bytes[SubresourceIndex];
            const auto& SubImage_UploadFootprint = _SubImages_UploadFootprint[SubresourceIndex];
            _TransientUploadBufferRange.Upload(
                SubImage_Bytes,
                  SubImage_UploadFootprint.Offset - _TransientUploadBufferRange.BeginOffsetInBytes
            );
        }
    }   
    void F_DirectX12RHIUploadTexturePass::Release()
    {
        _SubImages_UploadFootprint = {};
        _NumSubresources = 0;
        _TransientUploadBufferRange = {};
        
        A_DirectX12RHIPassExtension::Release();
        A_RHIUploadTexturePass::Release();
    }

    void F_DirectX12RHIUploadTexturePass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        SubresourceBindingSet.push_back(
            F_DirectX12RHISubresourceBinding::MakeCore(
                F_DirectX12RHISubresourceReference::Make(
                    _TransientUploadBufferRange.GetBuffer().Weak(),
                    0
                ),
                F_RHIResourceAccess::MakeCopySrc()
            )
        );
        for (U32 SubresourceIndex = 0; SubresourceIndex < _NumSubresources; ++SubresourceIndex)
        {
            SubresourceBindingSet.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        GetTexture().Weak(),
                        SubresourceIndex
                    ),
                    F_RHIResourceAccess::MakeCopyDest()
                )
            );
        }
    }

    E_DirectX12RHIPassBatchType F_DirectX12RHIUploadTexturePass::GetPassBatchType()
    {
        return E_DirectX12RHIPassBatchType::GPU;
    }
}
#endif