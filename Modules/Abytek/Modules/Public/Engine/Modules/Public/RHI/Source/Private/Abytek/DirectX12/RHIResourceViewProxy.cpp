#include "Abytek/DirectX12/RHIResourceViewProxy.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIFormat.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIResourceViewProxy::LateBuildResourceView_DirectX12()
    {
    }
    void F_DirectX12RHIResourceViewProxy::LateBuildBufferView_DirectX12()
    {
    }
    void F_DirectX12RHIResourceViewProxy::LateBuildTextureView_DirectX12()
    {
    }
    void F_DirectX12RHIResourceViewProxy::LateBuildRTASView_DirectX12()
    {
    }
    void F_DirectX12RHIResourceViewProxy::Release()
    {
        _DescriptorRange = {};
        A_RHIResourceViewProxy::Release();
    }

    void F_DirectX12RHIResourceViewProxy::AssignDescriptorRange(const F_DirectX12RHIDescriptorRange& DescriptorRange)
    {
        _DescriptorRange = DescriptorRange;
    }

    DirectX12SharedAPIWrapper::F_D3D12DescriptorDesc F_DirectX12RHIResourceViewProxy::GetDescriptorDesc()
    {
        DirectX12SharedAPIWrapper::F_D3D12DescriptorDesc Result;
         
        const auto& Access = GetAccess();
        Result.Type = DirectX12SharedAPIWrapper::Conversions::RHIResourceGPUAccessToDescriptorType(Access.GPU);
        
        DXGI_FORMAT DXGIFormat = RHIFormatToD3DFormat(GetFormat());
        
        auto CastedResourceProxy = GetResourceProxy().FastCast<F_DirectX12RHIResourceProxy>();
        auto D3D12Resource = CastedResourceProxy->GetD3D12Resource();
        
        Result.D3D12Resource = D3D12Resource;

        auto Archetype = GetArchetype();
        if (FlagHas(Archetype, E_RHIResourceViewArchetype::BUFFER_VIEW))
        {
            const auto& BufferAspect = GetResourceProxy()->GetBufferAspect();
            const auto& BufferViewAspect = GetBufferViewAspect();
            
            switch (Result.Type)
            {
            case DirectX12SharedAPIWrapper::E_DescriptorType::SRV:
                {
                    ABYTEK_ENGINE_RHI_ASSERT(D3D12Resource) << "SRV requires a valid resource";

                    D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
                    SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                    SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

                    if (DXGIFormat == DXGI_FORMAT_UNKNOWN)
                    {
                        ABYTEK_ENGINE_RHI_ASSERT(BufferViewAspect.SizeInBytes) << "SRV buffer size in bytes cannot be zero";
                        ABYTEK_ENGINE_RHI_ASSERT(BufferViewAspect.StrideInBytes) << "SRV buffer stride in bytes cannot be zero";
                    
                        // Structured or raw buffer
                        U32 StrideInBytes = BufferViewAspect.StrideInBytes;
                        if (BufferViewAspect.RawAccess)
                        {
                            SRVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
                            SRVDesc.Buffer.StructureByteStride = 0;
                            SRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
                            StrideInBytes = sizeof(U32);
                        }
                        else
                        {
                            SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
                            SRVDesc.Buffer.StructureByteStride = BufferViewAspect.StrideInBytes;
                            SRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
                        }
                        SRVDesc.Buffer.FirstElement = BufferViewAspect.OffsetInBytes / StrideInBytes;
                        SRVDesc.Buffer.NumElements = static_cast<UINT>(BufferViewAspect.SizeInBytes / StrideInBytes);
                    }
                    else
                    {
                        ABYTEK_ENGINE_RHI_ASSERT(BufferViewAspect.SizeInBytes) << "SRV buffer size in bytes cannot be zero";
                        ABYTEK_ENGINE_RHI_ASSERT(BufferViewAspect.StrideInBytes) << "SRV buffer stride in bytes cannot be zero";
                    
                        // Typed buffer
                        SRVDesc.Format = DXGIFormat;
                        SRVDesc.Buffer.FirstElement = BufferViewAspect.OffsetInBytes / BufferViewAspect.StrideInBytes;
                        SRVDesc.Buffer.NumElements = static_cast<UINT>(BufferViewAspect.SizeInBytes / BufferViewAspect.StrideInBytes);
                        SRVDesc.Buffer.StructureByteStride = 0;
                        SRVDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
                    }
                
                    Result.D3D12SRVDesc = SRVDesc;
                }
                break;
            case DirectX12SharedAPIWrapper::E_DescriptorType::UAV:
                {
                    ABYTEK_ENGINE_RHI_ASSERT(D3D12Resource) << "UAV requires a valid resource";

                    D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
                    UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

                    if (DXGIFormat == DXGI_FORMAT_UNKNOWN)
                    {
                        ABYTEK_ENGINE_RHI_ASSERT(BufferViewAspect.SizeInBytes) << "UAV buffer size in bytes cannot be zero";
                        ABYTEK_ENGINE_RHI_ASSERT(BufferViewAspect.StrideInBytes) << "UAV buffer stride in bytes cannot be zero";
                    
                        // Structured or raw buffer
                        UAVDesc.Buffer.CounterOffsetInBytes = 0;
                        U32 StrideInBytes = BufferViewAspect.StrideInBytes;
                        if (BufferViewAspect.RawAccess)
                        {
                            UAVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
                            UAVDesc.Buffer.StructureByteStride = 0;
                            UAVDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
                            StrideInBytes = sizeof(U32);
                        }
                        else
                        {
                            UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
                            UAVDesc.Buffer.StructureByteStride = BufferViewAspect.StrideInBytes;
                            UAVDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
                        }
                        UAVDesc.Buffer.FirstElement = BufferViewAspect.OffsetInBytes / StrideInBytes;
                        UAVDesc.Buffer.NumElements = static_cast<UINT>(BufferViewAspect.SizeInBytes / StrideInBytes);
                    }
                    else
                    {
                        ABYTEK_ENGINE_RHI_ASSERT(BufferViewAspect.SizeInBytes) << "UAV buffer size in bytes cannot be zero";
                        ABYTEK_ENGINE_RHI_ASSERT(BufferViewAspect.StrideInBytes) << "UAV buffer stride in bytes cannot be zero";
                    
                        // Typed buffer
                        UAVDesc.Format = DXGIFormat;
                        UAVDesc.Buffer.FirstElement = BufferViewAspect.OffsetInBytes / BufferViewAspect.StrideInBytes;
                        UAVDesc.Buffer.NumElements = static_cast<UINT>(BufferViewAspect.SizeInBytes / BufferViewAspect.StrideInBytes);
                        UAVDesc.Buffer.StructureByteStride = 0;
                        UAVDesc.Buffer.CounterOffsetInBytes = 0;
                        UAVDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
                    }
                
                    Result.D3D12UAVDesc = UAVDesc;
                }
                break;
            case DirectX12SharedAPIWrapper::E_DescriptorType::CBV:
                {
                    ABYTEK_ENGINE_RHI_ASSERT(BufferViewAspect.SizeInBytes) << "CBV buffer size in bytes cannot be zero";
                    ABYTEK_ENGINE_RHI_ASSERT(BufferViewAspect.SizeInBytes % 256 == 0) << "CBV buffer size in bytes bust be 256-byte aligned";
                
                    D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc = {};
                    CBVDesc.BufferLocation = D3D12Resource->GetGPUVirtualAddress() + BufferViewAspect.OffsetInBytes;
                    CBVDesc.SizeInBytes = static_cast<UINT>((BufferViewAspect.SizeInBytes + 255) & ~255); // Must be 256-byte aligned
                
                    Result.D3D12CBVDesc = CBVDesc;
                }
                break;
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid resource access to init descriptor";
                break;
            }
        }
        else if (FlagHas(Archetype, E_RHIResourceViewArchetype::TEXTURE_VIEW))
        {
            const auto& TextureAspect = GetResourceProxy()->GetTextureAspect();
            const auto& TextureViewAspect = GetTextureViewAspect();
            
            switch (Result.Type)
            {
            case DirectX12SharedAPIWrapper::E_DescriptorType::SRV:
                {
                    D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
                    SRVDesc.Format = DXGIFormat;
                    SRVDesc.ViewDimension = DirectX12SharedAPIWrapper::Conversions::ParseD3D12SRVDimension(
                        TextureAspect.DimensionCount,
                        TextureViewAspect.Flags
                    );
                    SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

                    switch (SRVDesc.ViewDimension)
                    {
                    case D3D12_SRV_DIMENSION_TEXTURE1D:
                        SRVDesc.Texture1D.MostDetailedMip = TextureViewAspect.FirstMipLevel;
                        SRVDesc.Texture1D.MipLevels = TextureViewAspect.MipLevelCount;
                        SRVDesc.Texture1D.ResourceMinLODClamp = 0.0f;
                        break;
                    case D3D12_SRV_DIMENSION_TEXTURE1DARRAY:
                        SRVDesc.Texture1DArray.MostDetailedMip = TextureViewAspect.FirstMipLevel;
                        SRVDesc.Texture1DArray.MipLevels = TextureViewAspect.MipLevelCount;
                        SRVDesc.Texture1DArray.FirstArraySlice = TextureViewAspect.FirstArraySize;
                        SRVDesc.Texture1DArray.ArraySize = TextureViewAspect.ArraySize;
                        SRVDesc.Texture1DArray.ResourceMinLODClamp = 0.0f;
                        break;
                    case D3D12_SRV_DIMENSION_TEXTURE2D:
                        SRVDesc.Texture2D.MostDetailedMip = TextureViewAspect.FirstMipLevel;
                        SRVDesc.Texture2D.MipLevels = TextureViewAspect.MipLevelCount;
                        SRVDesc.Texture2D.PlaneSlice = 0;
                        SRVDesc.Texture2D.ResourceMinLODClamp = 0.0f;
                        break;
                    case D3D12_SRV_DIMENSION_TEXTURE2DARRAY:
                        SRVDesc.Texture2DArray.MostDetailedMip = TextureViewAspect.FirstMipLevel;
                        SRVDesc.Texture2DArray.MipLevels = TextureViewAspect.MipLevelCount;
                        SRVDesc.Texture2DArray.FirstArraySlice = TextureViewAspect.FirstArraySize;
                        SRVDesc.Texture2DArray.ArraySize = TextureViewAspect.ArraySize;
                        SRVDesc.Texture2DArray.PlaneSlice = 0;
                        SRVDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
                        break;
                    case D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY:
                        SRVDesc.Texture2DMSArray.FirstArraySlice = TextureViewAspect.FirstArraySize;
                        SRVDesc.Texture2DMSArray.ArraySize = TextureViewAspect.ArraySize;
                        break;
                    case D3D12_SRV_DIMENSION_TEXTURE3D:
                        SRVDesc.Texture3D.MostDetailedMip = TextureViewAspect.FirstMipLevel;
                        SRVDesc.Texture3D.MipLevels = TextureViewAspect.MipLevelCount;
                        SRVDesc.Texture3D.ResourceMinLODClamp = 0.0f;
                        break;
                    case D3D12_SRV_DIMENSION_TEXTURECUBE:
                        SRVDesc.TextureCube.MostDetailedMip = TextureViewAspect.FirstMipLevel;
                        SRVDesc.TextureCube.MipLevels = TextureViewAspect.MipLevelCount;
                        SRVDesc.TextureCube.ResourceMinLODClamp = 0.0f;
                        break;
                    case D3D12_SRV_DIMENSION_TEXTURECUBEARRAY:
                        SRVDesc.TextureCubeArray.MostDetailedMip = TextureViewAspect.FirstMipLevel;
                        SRVDesc.TextureCubeArray.MipLevels = TextureViewAspect.MipLevelCount;
                        SRVDesc.TextureCubeArray.First2DArrayFace = TextureViewAspect.FirstArraySize;
                        SRVDesc.TextureCubeArray.NumCubes = TextureViewAspect.ArraySize / 6;
                        SRVDesc.TextureCubeArray.ResourceMinLODClamp = 0.0f;
                        break;
                    default:
                        ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid dimension";
                        break;
                    }
                
                    Result.D3D12SRVDesc = SRVDesc;
                }
                break;
            case DirectX12SharedAPIWrapper::E_DescriptorType::UAV:
                {
                    D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
                    UAVDesc.Format = DXGIFormat;
                    UAVDesc.ViewDimension = DirectX12SharedAPIWrapper::Conversions::ParseD3D12UAVDimension(
                        TextureAspect.DimensionCount,
                        TextureViewAspect.Flags
                    );

                    switch (UAVDesc.ViewDimension)
                    {
                    case D3D12_UAV_DIMENSION_TEXTURE1D:
                        UAVDesc.Texture1D.MipSlice = TextureViewAspect.FirstMipLevel;
                        break;
                    case D3D12_UAV_DIMENSION_TEXTURE1DARRAY:
                        UAVDesc.Texture1DArray.MipSlice = TextureViewAspect.FirstMipLevel;
                        UAVDesc.Texture1DArray.FirstArraySlice = TextureViewAspect.FirstArraySize;
                        UAVDesc.Texture1DArray.ArraySize = TextureViewAspect.ArraySize;
                        break;
                    case D3D12_UAV_DIMENSION_TEXTURE2D:
                        UAVDesc.Texture2D.MipSlice = TextureViewAspect.FirstMipLevel;
                        UAVDesc.Texture2D.PlaneSlice = 0;
                        break;
                    case D3D12_UAV_DIMENSION_TEXTURE2DARRAY:
                        UAVDesc.Texture2DArray.MipSlice = TextureViewAspect.FirstMipLevel;
                        UAVDesc.Texture2DArray.FirstArraySlice = TextureViewAspect.FirstArraySize;
                        UAVDesc.Texture2DArray.ArraySize = TextureViewAspect.ArraySize;
                        UAVDesc.Texture2DArray.PlaneSlice = 0;
                        break;
                    case D3D12_UAV_DIMENSION_TEXTURE3D:
                        UAVDesc.Texture3D.MipSlice = TextureViewAspect.FirstMipLevel;
                        UAVDesc.Texture3D.FirstWSlice = 0;
                        UAVDesc.Texture3D.WSize = TextureAspect.Depth;
                        break;
                    default:
                        ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid dimension";
                        break;
                    }
                
                    Result.D3D12UAVDesc = UAVDesc;
                }
                break;
            case DirectX12SharedAPIWrapper::E_DescriptorType::RTV:
                {
                    D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = {};
                    RTVDesc.Format = DXGIFormat;
                    RTVDesc.ViewDimension = DirectX12SharedAPIWrapper::Conversions::ParseD3D12RTVDimension(
                        TextureAspect.DimensionCount,
                        TextureViewAspect.Flags
                    );

                    switch (RTVDesc.ViewDimension)
                    {
                    case D3D12_RTV_DIMENSION_TEXTURE1D:
                        RTVDesc.Texture1D.MipSlice = TextureViewAspect.FirstMipLevel;
                        break;
                    case D3D12_RTV_DIMENSION_TEXTURE1DARRAY:
                        RTVDesc.Texture1DArray.MipSlice = TextureViewAspect.FirstMipLevel;
                        RTVDesc.Texture1DArray.FirstArraySlice = TextureViewAspect.FirstArraySize;
                        RTVDesc.Texture1DArray.ArraySize = TextureViewAspect.ArraySize;
                        break;
                    case D3D12_RTV_DIMENSION_TEXTURE2D:
                        RTVDesc.Texture2D.MipSlice = TextureViewAspect.FirstMipLevel;
                        RTVDesc.Texture2D.PlaneSlice = 0;
                        break;
                    case D3D12_RTV_DIMENSION_TEXTURE2DARRAY:
                        RTVDesc.Texture2DArray.MipSlice = TextureViewAspect.FirstMipLevel;
                        RTVDesc.Texture2DArray.FirstArraySlice = TextureViewAspect.FirstArraySize;
                        RTVDesc.Texture2DArray.ArraySize = TextureViewAspect.ArraySize;
                        RTVDesc.Texture2DArray.PlaneSlice = 0;
                        break;
                    case D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY:
                        RTVDesc.Texture2DMSArray.FirstArraySlice = TextureViewAspect.FirstArraySize;
                        RTVDesc.Texture2DMSArray.ArraySize = TextureViewAspect.ArraySize;
                        break;
                    case D3D12_RTV_DIMENSION_TEXTURE3D:
                        RTVDesc.Texture3D.MipSlice = TextureViewAspect.FirstMipLevel;
                        RTVDesc.Texture3D.FirstWSlice = 0;
                        RTVDesc.Texture3D.WSize = TextureAspect.Depth;
                        break;
                    default:
                        ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid dimension";
                        break;
                    }
                
                    Result.D3D12RTVDesc = RTVDesc;
                }
                break;
            case DirectX12SharedAPIWrapper::E_DescriptorType::DSV:
                {
                    D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
                    DSVDesc.Format = DXGIFormat;
                    DSVDesc.ViewDimension = DirectX12SharedAPIWrapper::Conversions::ParseD3D12DSVDimension(
                        TextureAspect.DimensionCount,
                        TextureViewAspect.Flags
                    );
                    DSVDesc.Flags = D3D12_DSV_FLAG_NONE;

                    switch (DSVDesc.ViewDimension)
                    {
                    case D3D12_DSV_DIMENSION_TEXTURE1D:
                        DSVDesc.Texture1D.MipSlice = TextureViewAspect.FirstMipLevel;
                        break;
                    case D3D12_DSV_DIMENSION_TEXTURE1DARRAY:
                        DSVDesc.Texture1DArray.MipSlice = TextureViewAspect.FirstMipLevel;
                        DSVDesc.Texture1DArray.FirstArraySlice = TextureViewAspect.FirstArraySize;
                        DSVDesc.Texture1DArray.ArraySize = TextureViewAspect.ArraySize;
                        break;
                    case D3D12_DSV_DIMENSION_TEXTURE2D:
                        DSVDesc.Texture2D.MipSlice = TextureViewAspect.FirstMipLevel;
                        break;
                    case D3D12_DSV_DIMENSION_TEXTURE2DARRAY:
                        DSVDesc.Texture2DArray.MipSlice = TextureViewAspect.FirstMipLevel;
                        DSVDesc.Texture2DArray.FirstArraySlice = TextureViewAspect.FirstArraySize;
                        DSVDesc.Texture2DArray.ArraySize = TextureViewAspect.ArraySize;
                        break;
                    case D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY:
                        DSVDesc.Texture2DMSArray.FirstArraySlice = TextureViewAspect.FirstArraySize;
                        DSVDesc.Texture2DMSArray.ArraySize = TextureViewAspect.ArraySize;
                        break;
                    default:
                        ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid dimension";
                        break;
                    }
                
                    Result.D3D12DSVDesc = DSVDesc;
                }
                break;
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid resource access to init descriptor";
                break;
            }
        }
        else if (FlagHas(Archetype, E_RHIResourceViewArchetype::RTAS_VIEW))
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "RTAS was not supported";
        }
        else
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid resource view archetype";
        }
        
        return Result;
    }
}
#endif