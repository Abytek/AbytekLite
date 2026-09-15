#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIBlendConfig.hpp"
#include "Abytek/RHICompareOperation.hpp"
#include "Abytek/RHIFormat.hpp"
#include "Abytek/RHIGPUWorkClass.hpp"
#include "Abytek/RHIPrimitiveTopology.hpp"
#include "Abytek/RHIRasterizerConfig.hpp"
#include "Abytek/RHIShaderFrequency.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHITextureFilter.hpp"


namespace Abytek
{
    class A_RHIResource;
    
    // For neccessary DirectX12 API wrapper for better C++ architecture
    namespace DirectX12SharedAPIWrapper
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        namespace Conversions
        {
            inline D3D12_RESOURCE_DIMENSION RHITextureDimensionCountToD3D12ResourceDimension(U32 DimensionCount)
            {
                static D3D12_RESOURCE_DIMENSION D3D12ResourceDimensions[] = {
                    D3D12_RESOURCE_DIMENSION_TEXTURE1D,
                    D3D12_RESOURCE_DIMENSION_TEXTURE2D,
                    D3D12_RESOURCE_DIMENSION_TEXTURE3D
                };
                ABYTEK_ENGINE_RHI_ASSERT(DimensionCount <= GetStaticArraySize(D3D12ResourceDimensions)) << "Invalid dimension count: " << DimensionCount;
                return D3D12ResourceDimensions[DimensionCount - 1];
            }
            inline D3D12_SRV_DIMENSION ParseD3D12SRVDimension(
                U32 DimensionCount,
                E_RHITextureFlag TextureFlags
            )
            {
                B8 IsArray = FlagHas(TextureFlags, E_RHITextureFlag::ARRAY);
                B8 IsCube = FlagHas(TextureFlags, E_RHITextureFlag::CUBE);
                
                switch (DimensionCount)
                {
                case 1:
                    if (IsArray)
                    {
                        return D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
                    }
                    else
                    {
                        return D3D12_SRV_DIMENSION_TEXTURE1D;
                    }
                    break;
                case 2:
                    if (IsArray)
                    {
                        if (IsCube)
                        {
                            return D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
                        }
                        else
                        {
                            return D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
                        }
                    }
                    else
                    {
                        if (IsCube)
                        {
                            return D3D12_SRV_DIMENSION_TEXTURECUBE;
                        }
                        else
                        {
                            return D3D12_SRV_DIMENSION_TEXTURE2D;
                        }
                    }
                    break;
                case 3:
                    return D3D12_SRV_DIMENSION_TEXTURE3D;
                    break;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid dimension count";
                    break;
                }
                return D3D12_SRV_DIMENSION_UNKNOWN;
            }
            inline D3D12_UAV_DIMENSION ParseD3D12UAVDimension(
                U32 DimensionCount,
                E_RHITextureFlag TextureFlags
            )
            {
                B8 IsArray = FlagHas(TextureFlags, E_RHITextureFlag::ARRAY);
                
                switch (DimensionCount)
                {
                case 1:
                    if (IsArray)
                    {
                        return D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
                    }
                    else
                    {
                        return D3D12_UAV_DIMENSION_TEXTURE1D;
                    }
                    break;
                case 2:
                    if (IsArray)
                    {
                        return D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
                    }
                    else
                    {
                        return D3D12_UAV_DIMENSION_TEXTURE2D;
                    }
                    break;
                case 3:
                    return D3D12_UAV_DIMENSION_TEXTURE3D;
                    break;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid dimension count";
                    break;
                }
                return D3D12_UAV_DIMENSION_UNKNOWN;
            }
            inline D3D12_RTV_DIMENSION ParseD3D12RTVDimension(
                U32 DimensionCount,
                E_RHITextureFlag TextureFlags
            )
            {
                B8 IsArray = FlagHas(TextureFlags, E_RHITextureFlag::ARRAY);
                
                switch (DimensionCount)
                {
                case 1:
                    if (IsArray)
                    {
                        return D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
                    }
                    else
                    {
                        return D3D12_RTV_DIMENSION_TEXTURE1D;
                    }
                    break;
                case 2:
                    if (IsArray)
                    {
                        return D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
                    }
                    else
                    {
                        return D3D12_RTV_DIMENSION_TEXTURE2D;
                    }
                    break;
                case 3:
                    return D3D12_RTV_DIMENSION_TEXTURE3D;
                    break;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid dimension count";
                    break;
                }
                return D3D12_RTV_DIMENSION_UNKNOWN;
            }
            inline D3D12_DSV_DIMENSION ParseD3D12DSVDimension(
                U32 DimensionCount,
                E_RHITextureFlag TextureFlags
            )
            {
                B8 IsArray = FlagHas(TextureFlags, E_RHITextureFlag::ARRAY);
                
                switch (DimensionCount)
                {
                case 1:
                    if (IsArray)
                    {
                        return D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
                    }
                    else
                    {
                        return D3D12_DSV_DIMENSION_TEXTURE1D;
                    }
                    break;
                case 2:
                    if (IsArray)
                    {
                        return D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
                    }
                    else
                    {
                        return D3D12_DSV_DIMENSION_TEXTURE2D;
                    }
                    break;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid dimension count";
                    break;
                }
                return D3D12_DSV_DIMENSION_UNKNOWN;
            }
        }
#endif
        
        enum class E_CommandListType : U8
        {
            NONE,
            DIRECT,
            COMPUTE,
            COPY,
        
            DEFAULT = DIRECT,
        };
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        namespace Conversions
        {
            inline D3D12_COMMAND_LIST_TYPE CommandListTypeToD3D12CommandListType(E_CommandListType A)
            {
                switch (A)
                {
                case E_CommandListType::DIRECT:
                    return D3D12_COMMAND_LIST_TYPE_DIRECT;
                case E_CommandListType::COMPUTE:
                    return D3D12_COMMAND_LIST_TYPE_COMPUTE;
                case E_CommandListType::COPY:
                    return D3D12_COMMAND_LIST_TYPE_COPY;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown command list type";
                }
                return D3D12_COMMAND_LIST_TYPE_NONE;
            }
        }
#endif
        namespace Conversions
        {
            inline E_CommandListType RHIGPUWorkClassToCommandListType(E_RHIGPUWorkClass GPUWorkClass)
            {
                switch (GPUWorkClass)
                {
                case E_RHIGPUWorkClass::DIRECT:
                    return E_CommandListType::DIRECT;
                case E_RHIGPUWorkClass::COMPUTE:
                    return E_CommandListType::COMPUTE;
                case E_RHIGPUWorkClass::BLIT:
                    return E_CommandListType::COPY;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown GPU work class";
                }
                return E_CommandListType::NONE;
            }
        }
        
        enum class E_CommandQueueFlag : U8
        {
            NONE = 0x0,
            DISABLE_GPU_TIMEOUT = 0x1,
        
            DEFAULT = NONE,
        };
        ABYTEK_DEFINE_FLAG_OPERATORS(E_CommandQueueFlag);
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        namespace Conversions
        {
            inline D3D12_COMMAND_QUEUE_FLAGS CommandQueueFlagToD3D12CommandQueueFlags(E_CommandQueueFlag A)
            {
                D3D12_COMMAND_QUEUE_FLAGS Result = D3D12_COMMAND_QUEUE_FLAG_NONE; 
                if (FlagHas(A, E_CommandQueueFlag::DISABLE_GPU_TIMEOUT))
                {
                    Result |= D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT;
                }
                return Result;
            }
        }
#endif
        
        enum class E_ShaderVisibility : U16
        {
            NONE,
            VERTEX,
            HULL,
            DOMAIN_,
            GEOMETRY,
            PIXEL,
            AMPLIFICATION,
            MESH,
            ALL
        };
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        namespace Conversions
        {
            inline D3D12_SHADER_VISIBILITY ShaderVisibilityToD3D12ShaderVisibility(E_ShaderVisibility A)
            {
                switch (A)
                {
                case E_ShaderVisibility::VERTEX:
                    return D3D12_SHADER_VISIBILITY_VERTEX;
                case E_ShaderVisibility::HULL:
                    return D3D12_SHADER_VISIBILITY_HULL;
                case E_ShaderVisibility::DOMAIN_:
                    return D3D12_SHADER_VISIBILITY_DOMAIN;
                case E_ShaderVisibility::GEOMETRY:
                    return D3D12_SHADER_VISIBILITY_GEOMETRY;
                case E_ShaderVisibility::PIXEL:
                    return D3D12_SHADER_VISIBILITY_PIXEL;
                case E_ShaderVisibility::AMPLIFICATION:
                    return D3D12_SHADER_VISIBILITY_AMPLIFICATION;
                case E_ShaderVisibility::MESH:
                    return D3D12_SHADER_VISIBILITY_MESH;
                case E_ShaderVisibility::ALL:
                    return D3D12_SHADER_VISIBILITY_ALL;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown shader visibility";
                }
                return D3D12_SHADER_VISIBILITY_ALL;
            }
        }
#endif
        namespace Conversions
        {
            inline E_ShaderVisibility RHIShaderFrequencyToShaderVisibility(E_RHIShaderFrequency ShaderFrequency)
            {
                switch (ShaderFrequency)
                {
                case E_RHIShaderFrequency::VERTEX:
                    return E_ShaderVisibility::VERTEX;
                case E_RHIShaderFrequency::PIXEL:
                    return E_ShaderVisibility::PIXEL;
                case E_RHIShaderFrequency::COMPUTE:
                    return E_ShaderVisibility::ALL;
                case E_RHIShaderFrequency::RT_RAY_GEN:
                    return E_ShaderVisibility::ALL;
                case E_RHIShaderFrequency::RT_CLOSEST_HIT:
                    return E_ShaderVisibility::ALL;
                case E_RHIShaderFrequency::RT_ANY_HIT:
                    return E_ShaderVisibility::ALL;
                case E_RHIShaderFrequency::RT_MISS:
                    return E_ShaderVisibility::ALL;
                case E_RHIShaderFrequency::WORK_GRAPH:
                    return E_ShaderVisibility::ALL;
                case E_RHIShaderFrequency::AMPLIFICATION:
                    return E_ShaderVisibility::AMPLIFICATION;
                case E_RHIShaderFrequency::MESH:
                    return E_ShaderVisibility::MESH;
                }
                return E_ShaderVisibility::ALL;
            }
        }

        enum class E_DescriptorHeapType : U8
        {
            NONE,
            CBV_SRV_UAV,
            SAMPLER,
            RTV,
            DSV
        };
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        namespace Conversions
        {
            inline D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapTypeToD3D12DescriptorHeapType(E_DescriptorHeapType A)
            {
                switch (A)
                {
                case E_DescriptorHeapType::NONE:
                    return D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
                case E_DescriptorHeapType::CBV_SRV_UAV:
                    return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
                case E_DescriptorHeapType::SAMPLER:
                    return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
                case E_DescriptorHeapType::RTV:
                    return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
                case E_DescriptorHeapType::DSV:
                    return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown descriptor heap type";
                }
                return D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
            }
        }
#endif
        namespace Conversions
        {
            inline E_DescriptorHeapType RHIResourceGPUAccessToDescriptorHeapType(E_RHIResourceGPUAccess GPUAccess)
            {
                if (
                    FlagHas(GPUAccess, E_RHIResourceGPUAccess::CBV)
                    || FlagHas(GPUAccess, E_RHIResourceGPUAccess::SRV)
                    || FlagHas(GPUAccess, E_RHIResourceGPUAccess::UAV)
                )
                {
                    return E_DescriptorHeapType::CBV_SRV_UAV;
                }
                if (FlagHas(GPUAccess, E_RHIResourceGPUAccess::RTV))
                {
                    return E_DescriptorHeapType::RTV;
                }
                if (FlagHas(GPUAccess, E_RHIResourceGPUAccess::DSV))
                {
                    return E_DescriptorHeapType::DSV;
                }
                return E_DescriptorHeapType::NONE;
            }
        }
        
        enum class E_DescriptorRangeType : U8
        {
            NONE,
            CBV,
            SRV,
            UAV,
            SAMPLER
        };
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        namespace Conversions
        {
            inline D3D12_DESCRIPTOR_RANGE_TYPE DescriptorRangeTypeToD3D12DescriptorRangeType(E_DescriptorRangeType A)
            {
                switch (A)
                {
                case E_DescriptorRangeType::NONE:
                    return (D3D12_DESCRIPTOR_RANGE_TYPE)0;
                case E_DescriptorRangeType::CBV:
                    return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                case E_DescriptorRangeType::SRV:
                    return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                case E_DescriptorRangeType::UAV:
                    return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
                case E_DescriptorRangeType::SAMPLER:
                    return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown descriptor range type";
                }
                return (D3D12_DESCRIPTOR_RANGE_TYPE)0;
            }
        }
#endif
        namespace Conversions
        {
            inline E_DescriptorHeapType DescriptorRangeTypeToDescriptorHeapType(E_DescriptorRangeType A)
            {
                switch (A)
                {
                case E_DescriptorRangeType::CBV:
                    return E_DescriptorHeapType::CBV_SRV_UAV;
                case E_DescriptorRangeType::SRV:
                    return E_DescriptorHeapType::CBV_SRV_UAV;
                case E_DescriptorRangeType::UAV:
                    return E_DescriptorHeapType::CBV_SRV_UAV;
                case E_DescriptorRangeType::SAMPLER:
                    return E_DescriptorHeapType::SAMPLER;
                }
                return E_DescriptorHeapType::NONE;
            }
        }
        
        struct F_DescriptorRange
        {
            E_DescriptorRangeType Type = E_DescriptorRangeType::NONE;
            U32 NumDescriptors = 0;
            U32 BaseShaderRegister = 0;
            U32 RegisterSpace = 0;
            U32 OffsetInDescriptorsFromTableStart = 0;
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_DescriptorRange& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Type);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.NumDescriptors);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.BaseShaderRegister);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RegisterSpace);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.OffsetInDescriptorsFromTableStart);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_DescriptorRange& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Type);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.NumDescriptors);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.BaseShaderRegister);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RegisterSpace);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.OffsetInDescriptorsFromTableStart);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct F_RootDescriptorTable
        {
            TF_SmallVector<F_DescriptorRange, 1> Ranges;
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RootDescriptorTable& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Ranges);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RootDescriptorTable& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Ranges);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct F_RootDescriptor
        {
            U32 ShaderRegister = 0;
            U32 RegisterSpace = 0;
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RootDescriptor& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ShaderRegister);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RegisterSpace);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RootDescriptor& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ShaderRegister);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RegisterSpace);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct F_RootConstants
        {
            U32 ShaderRegister = 0;
            U32 RegisterSpace = 0;
            U32 Num32BitValues = 0;
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RootConstants& Value) noexcept
            {
                View << Value.ShaderRegister;
                View << Value.RegisterSpace;
                View << Value.Num32BitValues;
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RootConstants& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ShaderRegister);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RegisterSpace);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Num32BitValues);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
         
        static constexpr U32 DefaultMaxRootConstantsSizeInBytes = 128;
        static constexpr U32 MinConstantBufferRegionAlignment = 256;
        
        enum class E_RootParameterType : U8
        {
            NONE,
            CBV,
            SRV,
            UAV,
            DESCRIPTOR_TABLE,
            CONSTANTS
        };
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        namespace Conversions
        {
            inline D3D12_ROOT_PARAMETER_TYPE RootParameterTypeToD3D12RootParameterType(E_RootParameterType A)
            {
                switch (A)
                {
                case E_RootParameterType::NONE:
                    return (D3D12_ROOT_PARAMETER_TYPE)0;
                case E_RootParameterType::CBV:
                    return D3D12_ROOT_PARAMETER_TYPE_CBV;
                case E_RootParameterType::SRV:
                    return D3D12_ROOT_PARAMETER_TYPE_SRV;
                case E_RootParameterType::UAV:
                    return D3D12_ROOT_PARAMETER_TYPE_UAV;
                case E_RootParameterType::DESCRIPTOR_TABLE:
                    return D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                case E_RootParameterType::CONSTANTS:
                    return D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown root parameter type";
                }
                return (D3D12_ROOT_PARAMETER_TYPE)0;
            }
        }
#endif
        
        struct F_RootParameter
        {
            E_RootParameterType Type = E_RootParameterType::NONE;
            F_RootDescriptorTable DescriptorTable;
            F_RootDescriptor Descriptor;
            F_RootConstants Constants;
            E_ShaderVisibility ShaderVisibility = E_ShaderVisibility::NONE;
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RootParameter& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Type);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DescriptorTable);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Descriptor);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Constants);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ShaderVisibility);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RootParameter& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Type);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DescriptorTable);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Descriptor);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Constants);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ShaderVisibility);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };

        struct F_InputElementDesc
        {
            F_Name SemanticName;
            U32 SemanticIndex = 0;
            E_RHIFormat Format = E_RHIFormat::NONE;
            U32 InputSlot = 0;
            U32 AlignedByteOffset = 0;
            E_RHIVertexInputElementClass Class = E_RHIVertexInputElementClass::NONE;
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_InputElementDesc& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SemanticName);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SemanticIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Format);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.InputSlot);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.AlignedByteOffset);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Class);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_InputElementDesc& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SemanticName);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SemanticIndex);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Format);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.InputSlot);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.AlignedByteOffset);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Class);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        namespace Conversions
        {
            inline D3D12_INPUT_CLASSIFICATION RHIVertexInputElementClassToD3D12InputClassification(E_RHIVertexInputElementClass A)
            {
                switch (A)
                {
                case E_RHIVertexInputElementClass::PER_VERTEX_DATA:
                    return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
                case E_RHIVertexInputElementClass::PER_INSTANCE_DATA:
                    return D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown vertex input element class";
                    break;
                }
                return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            }
        }
#endif

        using F_VertexBufferView = F_RHIVertexBufferBinding;
        using F_IndexBufferView = F_RHIIndexBufferBinding;
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        namespace Conversions
        {
            inline D3D12_BLEND RHIBlendFactorToD3D12Blend(E_RHIBlendFactor A)
            {
                switch (A)
                {
                case E_RHIBlendFactor::ZERO:
                    return D3D12_BLEND_ZERO;
                case E_RHIBlendFactor::ONE:
                    return D3D12_BLEND_ONE;
                case E_RHIBlendFactor::SRC_COLOR:
                    return D3D12_BLEND_SRC_COLOR;
                case E_RHIBlendFactor::INV_SRC_COLOR:
                    return D3D12_BLEND_INV_SRC_COLOR;
                case E_RHIBlendFactor::SRC_ALPHA:
                    return D3D12_BLEND_SRC_ALPHA;
                case E_RHIBlendFactor::INV_SRC_ALPHA:
                    return D3D12_BLEND_INV_SRC_ALPHA;
                case E_RHIBlendFactor::DEST_ALPHA:
                    return D3D12_BLEND_DEST_ALPHA;
                case E_RHIBlendFactor::INV_DEST_ALPHA:
                    return D3D12_BLEND_INV_DEST_ALPHA;
                case E_RHIBlendFactor::DEST_COLOR:
                    return D3D12_BLEND_DEST_COLOR;
                case E_RHIBlendFactor::INV_DEST_COLOR:
                    return D3D12_BLEND_INV_DEST_COLOR;
                case E_RHIBlendFactor::SRC_ALPHA_SAT:
                    return D3D12_BLEND_SRC_ALPHA_SAT;
                case E_RHIBlendFactor::BLEND_FACTOR:
                    return D3D12_BLEND_BLEND_FACTOR;
                case E_RHIBlendFactor::INV_BLEND_FACTOR:
                    return D3D12_BLEND_INV_BLEND_FACTOR;
                case E_RHIBlendFactor::SRC1_COLOR:
                    return D3D12_BLEND_SRC1_COLOR;
                case E_RHIBlendFactor::INV_SRC1_COLOR:
                    return D3D12_BLEND_INV_SRC1_COLOR;
                case E_RHIBlendFactor::SRC1_ALPHA:
                    return D3D12_BLEND_SRC1_ALPHA;
                case E_RHIBlendFactor::INV_SRC1_ALPHA:
                    return D3D12_BLEND_INV_SRC1_ALPHA;
                case E_RHIBlendFactor::ALPHA_FACTOR:
                    return D3D12_BLEND_ALPHA_FACTOR;
                case E_RHIBlendFactor::INV_ALPHA_FACTOR:
                    return D3D12_BLEND_INV_ALPHA_FACTOR;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown blend factor";
                    break;
                }
                return D3D12_BLEND_ONE;
            }
            inline D3D12_BLEND_OP RHIBlendOperationToD3D12BlendOp(E_RHIBlendOperation A)
            {
                switch (A)
                {
                case E_RHIBlendOperation::ADD:
                    return D3D12_BLEND_OP_ADD;
                case E_RHIBlendOperation::SUBTRACT:
                    return D3D12_BLEND_OP_SUBTRACT;
                case E_RHIBlendOperation::REV_SUBTRACT:
                    return D3D12_BLEND_OP_REV_SUBTRACT;
                case E_RHIBlendOperation::MIN:
                    return D3D12_BLEND_OP_MIN;
                case E_RHIBlendOperation::MAX:
                    return D3D12_BLEND_OP_MAX;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown blend operation";
                    break;
                }
                return D3D12_BLEND_OP_ADD;
            }
            inline D3D12_COMPARISON_FUNC RHICompareOperationToD3D12ComparisonFunc(E_RHICompareOperation CompareOperation)
            {
                switch (CompareOperation)
                {
                case E_RHICompareOperation::NONE:
                    return D3D12_COMPARISON_FUNC_NONE;
                case E_RHICompareOperation::NEVER:
                    return D3D12_COMPARISON_FUNC_NEVER;
                case E_RHICompareOperation::LESS:
                    return D3D12_COMPARISON_FUNC_LESS;
                case E_RHICompareOperation::EQUAL:
                    return D3D12_COMPARISON_FUNC_EQUAL;
                case E_RHICompareOperation::LESS_EQUAL:
                    return D3D12_COMPARISON_FUNC_LESS_EQUAL;
                case E_RHICompareOperation::GREATER:
                    return D3D12_COMPARISON_FUNC_GREATER;
                case E_RHICompareOperation::NOT_EQUAL:
                    return D3D12_COMPARISON_FUNC_NOT_EQUAL;
                case E_RHICompareOperation::GREATER_EQUAL:
                    return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
                case E_RHICompareOperation::ALWAYS:
                    return D3D12_COMPARISON_FUNC_ALWAYS;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown compare operation";
                    break;
                }
                return {};
            }
            inline D3D_PRIMITIVE_TOPOLOGY RHIPrimitiveTopologyToD3DPrimitiveTopology(E_RHIPrimitiveTopology PrimitiveTopology)
            {
                switch (PrimitiveTopology)
                {
                case E_RHIPrimitiveTopology::POINT_LIST:
                    return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
                case E_RHIPrimitiveTopology::LINE_LIST:
                    return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
                case E_RHIPrimitiveTopology::LINE_STRIP:
                    return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
                case E_RHIPrimitiveTopology::TRIANGLE_LIST:
                    return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                case E_RHIPrimitiveTopology::TRIANGLE_STRIP:
                    return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
                case E_RHIPrimitiveTopology::LINE_LIST_ADJ:
                    return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
                case E_RHIPrimitiveTopology::LINE_STRIP_ADJ:
                    return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
                case E_RHIPrimitiveTopology::TRIANGLE_LIST_ADJ:
                    return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
                case E_RHIPrimitiveTopology::TRIANGLE_STRIP_ADJ:
                    return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown primitive topology";
                    break;
                }
                return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
            }
            inline D3D12_PRIMITIVE_TOPOLOGY_TYPE RHIPrimitiveTopologyToD3D12PrimitiveTopologyType(E_RHIPrimitiveTopology PrimitiveTopology)
            {
                switch (PrimitiveTopology)
                {
                case E_RHIPrimitiveTopology::POINT_LIST:
                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
                case E_RHIPrimitiveTopology::LINE_LIST:
                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
                case E_RHIPrimitiveTopology::LINE_STRIP:
                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
                case E_RHIPrimitiveTopology::TRIANGLE_LIST:
                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                case E_RHIPrimitiveTopology::TRIANGLE_STRIP:
                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                case E_RHIPrimitiveTopology::LINE_LIST_ADJ:
                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
                case E_RHIPrimitiveTopology::LINE_STRIP_ADJ:
                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
                case E_RHIPrimitiveTopology::TRIANGLE_LIST_ADJ:
                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                case E_RHIPrimitiveTopology::TRIANGLE_STRIP_ADJ:
                    return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown primitive topology";
                    break;
                }
                return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
            }
            inline D3D12_CULL_MODE RHICullModeToD3D12CullMode(E_RHICullMode CullMode)
            {
                switch (CullMode)
                {
                case E_RHICullMode::NONE:
                    return D3D12_CULL_MODE_NONE;
                case E_RHICullMode::FRONT:
                    return D3D12_CULL_MODE_FRONT;
                case E_RHICullMode::BACK:
                    return D3D12_CULL_MODE_BACK;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown cull mode";
                    break;
                }
                return D3D12_CULL_MODE_NONE;
            }
            inline D3D12_FILL_MODE RHIFillModeToD3D12FillMode(E_RHIFillMode FillMode)
            {
                switch (FillMode)
                {
                case E_RHIFillMode::WIREFRAME:
                    return D3D12_FILL_MODE_WIREFRAME;
                case E_RHIFillMode::SOLID:
                    return D3D12_FILL_MODE_SOLID;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown fill mode";
                    break;
                }
                return D3D12_FILL_MODE_WIREFRAME;
            }
            inline D3D12_CONSERVATIVE_RASTERIZATION_MODE RHIConservativeRasterModeToD3D12ConservativeRasterizationMode(E_RHIConservativeRasterMode A)
            {
                switch (A)
                {
                case E_RHIConservativeRasterMode::ENABLED:
                    return D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON;
                case E_RHIConservativeRasterMode::NONE:
                    return D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown conservative raster mode";
                    break;
                }
                return D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
            }
        }
#endif
        
        enum class E_DescriptorType : U8
        {
            NONE,
            CBV,
            SRV,
            UAV,
            RTV,
            DSV,
            SAMPLER,
            NUM
        };
        namespace Conversions
        {
            inline E_DescriptorType RHIResourceGPUAccessToDescriptorType(E_RHIResourceGPUAccess GPUAccess)
            {
                if (FlagHas(GPUAccess, E_RHIResourceGPUAccess::CBV))
                {
                    return E_DescriptorType::CBV;
                }
                if (FlagHas(GPUAccess, E_RHIResourceGPUAccess::SRV))
                {
                    return E_DescriptorType::SRV;
                }
                if (FlagHas(GPUAccess, E_RHIResourceGPUAccess::UAV))
                {
                    return E_DescriptorType::UAV;
                }
                if (FlagHas(GPUAccess, E_RHIResourceGPUAccess::RTV))
                {
                    return E_DescriptorType::RTV;
                }
                if (FlagHas(GPUAccess, E_RHIResourceGPUAccess::DSV))
                {
                    return E_DescriptorType::DSV;
                }
                return E_DescriptorType::NONE;
            }
        }
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        struct F_D3D12DescriptorDesc
        {
            E_DescriptorType Type = E_DescriptorType::NONE;
            Microsoft::WRL::ComPtr<ID3D12Resource> D3D12Resource = nullptr;
            union 
            {
                D3D12_CONSTANT_BUFFER_VIEW_DESC D3D12CBVDesc;
                D3D12_SHADER_RESOURCE_VIEW_DESC D3D12SRVDesc;
                D3D12_UNORDERED_ACCESS_VIEW_DESC D3D12UAVDesc;
                D3D12_RENDER_TARGET_VIEW_DESC D3D12RTVDesc;
                D3D12_DEPTH_STENCIL_VIEW_DESC D3D12DSVDesc;
                D3D12_SAMPLER_DESC D3D12SamplerDesc;
            };
        };
#endif
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        namespace Conversions
        {
            inline D3D12_HEAP_TYPE RHIResourceAccessToD3D12HeapType(const F_RHIResourceAccess& Access)
            {
                B8 HasCPUWrite = RHIHasCPUWriteAccess(Access.CPU);
                B8 HasCPUReadOnly = RHIHasCPUReadOnlyAccess(Access.CPU);
                B8 HasGPUWrite = RHIHasGPUWriteAccess(Access.GPU);
                B8 HasGPUReadOnly = RHIHasGPUReadOnlyAccess(Access.GPU);
                if (HasCPUReadOnly && HasGPUWrite)
                {
                    return D3D12_HEAP_TYPE_READBACK;
                }
                if (HasCPUWrite && HasGPUReadOnly)
                {
                    return D3D12_HEAP_TYPE_UPLOAD;
                }
                return D3D12_HEAP_TYPE_DEFAULT;
            }
        }
#endif
    }
}

namespace boost
{
    template<>
    struct hash<Abytek::DirectX12SharedAPIWrapper::F_DescriptorRange>
    {
        size_t operator ()(const Abytek::DirectX12SharedAPIWrapper::F_DescriptorRange& X) const
        {
            size_t Result = 0;
            hash_combine(Result, X.Type);
            hash_combine(Result, X.NumDescriptors);
            hash_combine(Result, X.BaseShaderRegister);
            hash_combine(Result, X.RegisterSpace);
            hash_combine(Result, X.OffsetInDescriptorsFromTableStart);
            return Result;
        }
    };
    template<>
    struct hash<Abytek::DirectX12SharedAPIWrapper::F_RootDescriptorTable>
    {
        size_t operator ()(const Abytek::DirectX12SharedAPIWrapper::F_RootDescriptorTable& X) const
        {
            size_t Result = 0;
            hash_combine(Result, X.Ranges);
            return Result;
        }
    };
    template<>
    struct hash<Abytek::DirectX12SharedAPIWrapper::F_RootDescriptor>
    {
        size_t operator ()(const Abytek::DirectX12SharedAPIWrapper::F_RootDescriptor& X) const
        {
            size_t Result = 0;
            hash_combine(Result, X.ShaderRegister);
            hash_combine(Result, X.RegisterSpace);
            return Result;
        }
    };
    template<>
    struct hash<Abytek::DirectX12SharedAPIWrapper::F_RootConstants>
    {
        size_t operator ()(const Abytek::DirectX12SharedAPIWrapper::F_RootConstants& X) const
        {
            size_t Result = 0;
            hash_combine(Result, X.ShaderRegister);
            hash_combine(Result, X.RegisterSpace);
            hash_combine(Result, X.Num32BitValues);
            return Result;
        }
    };
    template<>
    struct hash<Abytek::DirectX12SharedAPIWrapper::F_RootParameter>
    {
        size_t operator ()(const Abytek::DirectX12SharedAPIWrapper::F_RootParameter& X) const
        {
            size_t Result = 0;
            hash_combine(Result, X.Type);
            switch (X.Type)
            {
            case Abytek::DirectX12SharedAPIWrapper::E_RootParameterType::CBV:
            case Abytek::DirectX12SharedAPIWrapper::E_RootParameterType::SRV:
            case Abytek::DirectX12SharedAPIWrapper::E_RootParameterType::UAV:
                hash_combine(Result, X.Descriptor);
                break;
            case Abytek::DirectX12SharedAPIWrapper::E_RootParameterType::CONSTANTS:
                hash_combine(Result, X.Constants);
                break;
            case Abytek::DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE:
                hash_combine(Result, X.DescriptorTable);
                break;
            }
            return Result;
        }
    };
    template<>
    struct hash<Abytek::DirectX12SharedAPIWrapper::F_InputElementDesc>
    {
        size_t operator ()(const Abytek::DirectX12SharedAPIWrapper::F_InputElementDesc& X) const
        {
            size_t Result = 0;
            hash_combine(Result, X.SemanticName);
            hash_combine(Result, X.SemanticIndex);
            hash_combine(Result, X.Format);
            hash_combine(Result, X.InputSlot);
            hash_combine(Result, X.AlignedByteOffset);
            hash_combine(Result, X.Class);
            return Result;
        }
    };
}