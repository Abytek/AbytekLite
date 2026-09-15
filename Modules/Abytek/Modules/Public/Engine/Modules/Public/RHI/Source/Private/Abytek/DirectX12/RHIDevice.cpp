#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/DirectX12/RHIFormat.hpp"
#include "Abytek/RHISubsystem.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
#ifdef ABYTEK_DEBUG_INFO
    void F_DirectX12RHIDevice::SetDebugName(const F_DebugName& DebugName) noexcept
    {
        A_RAObject::SetDebugName(DebugName);
        if (_D3D12Device)
        {
            _D3D12Device->SetName(*DebugName);
        }
    }
#endif

    void F_DirectX12RHIDevice::Build()
    {
        A_RHIDevice::Build();
    }
    void F_DirectX12RHIDevice::Release()
    {
        _DXGIAdapter = nullptr;
        A_RHIDevice::Release();
    }

    void F_DirectX12RHIDevice::Activate()
    {
        A_RHIDevice::Activate();
        
        HRESULT HR;

        D3D_FEATURE_LEVEL FeatureLevels[] =
        {
            D3D_FEATURE_LEVEL_12_2,
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0
        };
        UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);

        for (UINT FeatureLevelIndex = 0; FeatureLevelIndex < NumFeatureLevels;)
        {
            HR = D3D12CreateDevice(
                _DXGIAdapter.Get(),
                FeatureLevels[FeatureLevelIndex],
                IID_PPV_ARGS(&_D3D12Device)
            );
            if (SUCCEEDED(HR))
            {
                _PostActivationSetup();
                break;
            }
            ++FeatureLevelIndex;
        }

        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "can't create d3d12 device";
        
        {
            Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
            if (SUCCEEDED(_D3D12Device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
            {
                D3D12_MESSAGE_ID denyIds[] =
                {
                    D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE
                };

                D3D12_INFO_QUEUE_FILTER filter = {};
                filter.DenyList.NumIDs = _countof(denyIds);
                filter.DenyList.pIDList = denyIds;

                infoQueue->AddStorageFilterEntries(&filter);
            }
        }

        ABYTEK_LOG_INFO() << "Created d3d12 device: " << GetInfo().Description;
    }
    void F_DirectX12RHIDevice::Deactivate()
    {
        _D3D12Device = nullptr;
        A_RHIDevice::Deactivate();
    }

    void F_DirectX12RHIDevice::_PostActivationSetup()
    {
    }

    F_RHIFeatureSet F_DirectX12RHIDevice::GenerateFeatureSet()
    {
        F_RHIFeatureSet Result;

        //-----------------------------------------
        // Shader model
        //-----------------------------------------

        D3D12_FEATURE_DATA_SHADER_MODEL ShaderModel = {};
        ShaderModel.HighestShaderModel = D3D_SHADER_MODEL_6_7;

        if (FAILED(_D3D12Device->CheckFeatureSupport(
            D3D12_FEATURE_SHADER_MODEL,
            &ShaderModel,
            sizeof(ShaderModel))))
        {
            ShaderModel.HighestShaderModel = D3D_SHADER_MODEL_5_1;
        }

        //-----------------------------------------
        // OPTIONS
        //-----------------------------------------

        D3D12_FEATURE_DATA_D3D12_OPTIONS Opt = {};
        _D3D12Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &Opt, sizeof(Opt));

        D3D12_FEATURE_DATA_D3D12_OPTIONS1 Opt1 = {};
        _D3D12Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &Opt1, sizeof(Opt1));

        D3D12_FEATURE_DATA_D3D12_OPTIONS2 Opt2 = {};
        _D3D12Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &Opt2, sizeof(Opt2));

        D3D12_FEATURE_DATA_D3D12_OPTIONS3 Opt3 = {};
        _D3D12Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &Opt2, sizeof(Opt3));

        D3D12_FEATURE_DATA_D3D12_OPTIONS5 Opt5 = {};
        _D3D12Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &Opt5, sizeof(Opt5));

        D3D12_FEATURE_DATA_D3D12_OPTIONS6 Opt6 = {};
        _D3D12Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &Opt6, sizeof(Opt6));

        D3D12_FEATURE_DATA_D3D12_OPTIONS7 Opt7 = {};
        _D3D12Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &Opt7, sizeof(Opt7));

        //-----------------------------------------
        // Ray tracing
        //-----------------------------------------

        if (Opt5.RaytracingTier >= D3D12_RAYTRACING_TIER_1_0)
        {
            Result.RayTracing = F_RHIFeatureSet::E_RayTracing::L1;
        }

        if (Opt5.RaytracingTier >= D3D12_RAYTRACING_TIER_1_1)
        {
            Result.RayTracing = F_RHIFeatureSet::E_RayTracing::L2;
        }

        //-----------------------------------------
        // Mesh shader
        //-----------------------------------------

        if (Opt7.MeshShaderTier != D3D12_MESH_SHADER_TIER_NOT_SUPPORTED)
        {
            Result.MeshShader = F_RHIFeatureSet::E_MeshShader::L1;
        }

        //-----------------------------------------
        // Variable Rate Shading
        //-----------------------------------------

        if (Opt6.VariableShadingRateTier >= D3D12_VARIABLE_SHADING_RATE_TIER_1)
        {
            Result.VariableRateShading =
                F_RHIFeatureSet::E_VariableRateShading::L1;
        }

        if (Opt6.VariableShadingRateTier >= D3D12_VARIABLE_SHADING_RATE_TIER_2)
        {
            Result.VariableRateShading =
                F_RHIFeatureSet::E_VariableRateShading::L2;
        }

        //-----------------------------------------
        // Sampler feedback
        //-----------------------------------------

        if (Opt7.SamplerFeedbackTier >= D3D12_SAMPLER_FEEDBACK_TIER_0_9)
        {
            Result.SamplerFeedback =
                F_RHIFeatureSet::E_SamplerFeedback::L1;
        }

        if (Opt7.SamplerFeedbackTier >= D3D12_SAMPLER_FEEDBACK_TIER_1_0)
        {
            Result.SamplerFeedback =
                F_RHIFeatureSet::E_SamplerFeedback::L2;
        }

        //-----------------------------------------
        // Conservative raster
        //-----------------------------------------

        if (Opt.ConservativeRasterizationTier >=
            D3D12_CONSERVATIVE_RASTERIZATION_TIER_1)
        {
            Result.ConservativeRaster =
                F_RHIFeatureSet::E_ConservativeRaster::L1;
        }

        if (Opt.ConservativeRasterizationTier >=
            D3D12_CONSERVATIVE_RASTERIZATION_TIER_2)
        {
            Result.ConservativeRaster =
                F_RHIFeatureSet::E_ConservativeRaster::L2;
        }

        if (Opt.ConservativeRasterizationTier >=
            D3D12_CONSERVATIVE_RASTERIZATION_TIER_3)
        {
            Result.ConservativeRaster =
                F_RHIFeatureSet::E_ConservativeRaster::L3;
        }

        //-----------------------------------------
        // Wave ops
        //-----------------------------------------

        if (Opt1.WaveOps)
        {
            Result.WaveOps = F_RHIFeatureSet::E_WaveOps::L1;
        }

        //-----------------------------------------
        // Atomic int64
        //-----------------------------------------

        if (Opt1.Int64ShaderOps)
        {
            Result.AtomicOps =
                F_RHIFeatureSet::E_AtomicOps::FEATURE_I64;
        }
        else
        {
            Result.AtomicOps =
                F_RHIFeatureSet::E_AtomicOps::L1;
        }

        //-----------------------------------------
        // UploadBufferDetachCopyPass
        //-----------------------------------------

        Result.UploadBufferDetachCopyPass = F_RHIFeatureSet::E_UploadBufferDetachCopyPass::SUPPORTED;

        //-----------------------------------------
        // UploadTextureDetachCopyPass
        //-----------------------------------------

        Result.UploadTextureDetachCopyPass = F_RHIFeatureSet::E_UploadTextureDetachCopyPass::SUPPORTED;

        //-----------------------------------------
        // FP16
        //-----------------------------------------

        if (Opt.MinPrecisionSupport &
            D3D12_SHADER_MIN_PRECISION_SUPPORT_16_BIT)
        {
            Result.PrimitiveTypes =
                F_RHIFeatureSet::E_PrimitiveTypes::FEATURE_F16;
        }
        else
        {
            Result.PrimitiveTypes =
                F_RHIFeatureSet::E_PrimitiveTypes::L1;
        }

        //-----------------------------------------
        // Binding model (rough heuristic)
        //-----------------------------------------

        if (Opt.ResourceBindingTier >= D3D12_RESOURCE_BINDING_TIER_1)
            Result.Binding = F_RHIFeatureSet::E_Binding::L1;

        if (Opt.ResourceBindingTier >= D3D12_RESOURCE_BINDING_TIER_2)
            Result.Binding = F_RHIFeatureSet::E_Binding::L2;

        if (Opt.ResourceBindingTier >= D3D12_RESOURCE_BINDING_TIER_3)
            Result.Binding = F_RHIFeatureSet::E_Binding::L3;

        //-----------------------------------------
        // MultiView (view instancing)
        //-----------------------------------------

        if (Opt3.ViewInstancingTier !=
            D3D12_VIEW_INSTANCING_TIER_NOT_SUPPORTED)
        {
            Result.MultiView = F_RHIFeatureSet::E_MultiView::L1;
        }

        // Add formats
        for (U32 UsageIdx = 0; UsageIdx < static_cast<U32>(F_RHIFeatureSet::E_FormatUsageType::NUM); ++UsageIdx)
        {
            auto UsageType = static_cast<F_RHIFeatureSet::E_FormatUsageType>(UsageIdx);
            if (UsageType == F_RHIFeatureSet::E_FormatUsageType::NONE)
            {
                continue;
            }

            F_RHIFeatureSet::F_FormatUsage Usage;
            
            D3D12_FORMAT_SUPPORT1 D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
            D3D12_FORMAT_SUPPORT2 D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
            switch (UsageType)
            {
            case F_RHIFeatureSet::E_FormatUsageType::BUFFER:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_BUFFER;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::IA_VERTEX_BUFFER:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_IA_VERTEX_BUFFER;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::IA_INDEX_BUFFER:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_IA_INDEX_BUFFER;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::SO_BUFFER:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_SO_BUFFER;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::TEXTURE1D:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_TEXTURE1D;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::TEXTURE2D:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_TEXTURE2D;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::TEXTURE3D:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_TEXTURE3D;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::TEXTURECUBE:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_TEXTURECUBE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::SHADER_LOAD:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_SHADER_LOAD;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::SHADER_SAMPLE:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::SHADER_SAMPLE_COMPARISON:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_COMPARISON;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::SHADER_SAMPLE_MONO_TEXT:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_MONO_TEXT;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::MIP:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_MIP;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::RENDER_TARGET:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_RENDER_TARGET;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::BLENDABLE:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_BLENDABLE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::DEPTH_STENCIL:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::MULTISAMPLE_RESOLVE:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RESOLVE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::DISPLAY:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_DISPLAY;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::CAST_WITHIN_BIT_LAYOUT:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_CAST_WITHIN_BIT_LAYOUT;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::MULTISAMPLE_RENDERTARGET:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RENDERTARGET;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::MULTISAMPLE_LOAD:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_MULTISAMPLE_LOAD;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::SHADER_GATHER:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_SHADER_GATHER;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::BACK_BUFFER_CAST:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_BACK_BUFFER_CAST;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::TYPED_UNORDERED_ACCESS_VIEW:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::SHADER_GATHER_COMPARISON:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_SHADER_GATHER_COMPARISON;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::DECODER_OUTPUT:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_DECODER_OUTPUT;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::VIDEO_PROCESSOR_OUTPUT:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_OUTPUT;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::VIDEO_PROCESSOR_INPUT:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_INPUT;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::VIDEO_ENCODER:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_VIDEO_ENCODER;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_NONE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::UAV_ATOMIC_ADD:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_ADD;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::UAV_ATOMIC_BITWISE_OPS:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::UAV_ATOMIC_EXCHANGE:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::UAV_ATOMIC_SIGNED_MIN_OR_MAX:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::UAV_ATOMIC_UNSIGNED_MIN_OR_MAX:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::UAV_TYPED_LOAD:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::UAV_TYPED_STORE:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::OUTPUT_MERGER_LOGIC_OP:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::TILED:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_TILED;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::MULTIPLANE_OVERLAY:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_MULTIPLANE_OVERLAY;
                break;
            case F_RHIFeatureSet::E_FormatUsageType::SAMPLER_FEEDBACK:
                D3D12FormatSupport1 = D3D12_FORMAT_SUPPORT1_NONE;
                D3D12FormatSupport2 = D3D12_FORMAT_SUPPORT2_SAMPLER_FEEDBACK;
                break;
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown usage type";
                break;
            }
            
            for (int FormatIdx = 0; FormatIdx < (int)E_RHIFormat::NUM; ++FormatIdx)
            {
                E_RHIFormat RHIFormat = (E_RHIFormat)FormatIdx;
                if (RHIFormat == E_RHIFormat::NONE)
                {
                    continue;
                }

                DXGI_FORMAT D3DFormat = RHIFormatToD3DFormat(RHIFormat);

                if (D3DFormat == DXGI_FORMAT_UNKNOWN)
                    continue;

                D3D12_FEATURE_DATA_FORMAT_SUPPORT Support = {};
                Support.Format = D3DFormat;

                if (FAILED(_D3D12Device->CheckFeatureSupport(
                    D3D12_FEATURE_FORMAT_SUPPORT,
                    &Support,
                    sizeof(Support))))
                {
                    continue;
                }

                if (
                    (Support.Support1 & D3D12FormatSupport1)
                    || (Support.Support2 & D3D12FormatSupport2)
                )
                {
                    Usage.Formats.insert(RHIFormat);
                }
            }

            Result.FormatUsages.push_back(Usage);
        }
        return A_RHIDevice::GenerateFeatureSet();
    }
}
#endif