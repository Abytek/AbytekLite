#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIShaderFrequency.hpp"
#include "Abytek/RHIFormat.hpp"


namespace Abytek
{
    enum class E_RHIFeatureModel
    {
        NONE,
        
        // ---------------------------------------------------------------------
        // DESKTOP_0
        //
        // Target: Baseline modern desktop GPUs (~2016 era)
        // Examples: NVIDIA Pascal (GTX 10xx), AMD Polaris, Intel Gen9
        //
        // Guaranteed capabilities:
        // - Shader Model 5.x class feature set
        // - Compute shaders
        // - Tessellation
        // - Geometry shaders
        // - Conservative rasterization (optional but commonly available)
        // - Bindless resources limited or emulated
        // - No hardware ray tracing
        //
        // Typical usage:
        // - Forward / Deferred rendering
        // - Compute based post processing
        // - Virtual texturing
        //
        // Constraints:
        // - Limited wave operations
        // - No mesh/task shaders
        // - No hardware RTAS traversal
        //
        DESKTOP_0,
        
        // ---------------------------------------------------------------------
        // DESKTOP_1
        //
        // Target: Modern desktop GPUs (~2018+)
        // Examples: NVIDIA Turing / Ampere / Ada, AMD RDNA2+, Intel Xe
        //
        // Guaranteed capabilities:
        // - Shader Model 6.x
        // - Wave / subgroup operations
        // - Descriptor indexing / bindless resources
        // - Hardware ray tracing acceleration
        // - Mesh shaders / task shaders (if supported by backend API)
        // - Variable rate shading
        //
        // Typical usage:
        // - GPU driven rendering
        // - Hardware ray tracing pipelines
        // - Advanced visibility systems
        // - Mesh shader pipelines
        //
        // Constraints:
        // - Assumes large VRAM and high compute throughput
        //
        DESKTOP_1,

        // ---------------------------------------------------------------------
        // XR_0
        //
        // Target: Standalone XR / mobile VR baseline
        // Examples: Meta Quest 2 / Snapdragon XR2 class GPUs
        //
        // Guaranteed capabilities:
        // - Mobile GPU architecture (tile-based rendering)
        // - Compute shaders (limited throughput)
        // - Multiview rendering
        // - Fixed foveated rendering
        // - ASTC texture compression
        //
        // Typical usage:
        // - Lightweight forward rendering
        // - Low bandwidth rendering techniques
        // - Multiview stereo rendering
        //
        // Constraints:
        // - Limited memory bandwidth
        // - No hardware ray tracing
        // - Very limited geometry amplification
        //
        XR_0,

        // ---------------------------------------------------------------------
        // XR_1
        //
        // Target: Next-gen XR devices
        // Examples: Future XR SoCs / PCVR hybrid systems
        //
        // Guaranteed capabilities:
        // - Advanced multiview / multiview instancing
        // - Eye tracked foveated rendering
        // - Higher compute throughput
        // - Improved bindless resource support
        //
        // Typical usage:
        // - Advanced VR rendering pipelines
        // - GPU driven stereo rendering
        // - More complex lighting / post effects
        //
        // Constraints:
        // - Still optimized for power efficiency
        // - Geometry and memory budgets smaller than desktop GPUs
        //
        XR_1,
        
        MOBILE_0,

        NUM,
#ifdef ABYTEK_PLATFORM_WINDOWS
        DEFAULT = DESKTOP_0
#else
        DEFAULT = MOBILE_0 // lowest
#endif
    };
    
    struct F_RHIFeatureSet
    {
        enum class E_RayTracing : U8
        {
            NONE = 0x0,
            
            // Basic ray tracing pipeline
            // - RayGen / Miss / ClosestHit
            // - Acceleration structures
            // - Basic ray queries
            L1 = 0x1,

            // Advanced ray tracing
            // - Inline ray tracing
            // - Indirect ray dispatch
            // - Shader table improvements
            L2 = 0x2
        } RayTracing = E_RayTracing::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_RayTracing);
        static E_RayTracing GetMinimalRayTracing(E_RHIFeatureModel Model)
        {
            if (Model == E_RHIFeatureModel::DESKTOP_1)
            {
                return E_RayTracing::L1;
            }
            return E_RayTracing::NONE;
        }
        
        enum class E_MeshShader : U8
        {
            NONE = 0x0,

            // Basic mesh shader pipeline
            // - Mesh shader
            // - Amplification shader
            // - Meshlets
            L1 = 0x1
        } MeshShader = E_MeshShader::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_MeshShader);
        static E_MeshShader GetMinimalMeshShader(E_RHIFeatureModel Model)
        {
            if (Model == E_RHIFeatureModel::DESKTOP_1)
            {
                return E_MeshShader::L1;
            }
            return E_MeshShader::NONE;
        }
        
        enum class E_VariableRateShading : U8
        {
            NONE = 0x0,

            // Per-draw shading rate
            // Fixed rate for whole draw call
            L1 = 0x1,

            // Per-primitive / image based shading
            // Shading rate texture
            L2 = 0x2
        } VariableRateShading = E_VariableRateShading::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_VariableRateShading);
        static E_VariableRateShading GetMinimalVariableRateShading(E_RHIFeatureModel Model)
        {
            if (Model == E_RHIFeatureModel::DESKTOP_1)
            {
                return E_VariableRateShading::L1;
            }
            return E_VariableRateShading::NONE;
        }
        
        enum class E_SamplerFeedback : U8
        {
            NONE = 0x0,

            // Basic sampler feedback
            // - Record mip levels accessed
            // - Texture streaming support
            L1 = 0x1,

            // Advanced streaming feedback
            // - Tile feedback
            // - Virtual texture optimization
            L2 = 0x2
        } SamplerFeedback = E_SamplerFeedback::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_SamplerFeedback);
        static E_SamplerFeedback GetMinimalSamplerFeedback(E_RHIFeatureModel Model)
        {
            return E_SamplerFeedback::NONE;
        }
        
        enum class E_ConservativeRaster : U8
        {
            NONE = 0x0,

            // Basic conservative raster
            // pixel covered if triangle overlaps
            L1 = 0x1,

            // Post-snap raster improvements
            L2 = 0x2,

            // Fully programmable conservative raster
            L3 = 0x3
        } ConservativeRaster = E_ConservativeRaster::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_ConservativeRaster);
        static E_ConservativeRaster GetMinimalConservativeRaster(E_RHIFeatureModel Model)
        {
            if (Model == E_RHIFeatureModel::DESKTOP_1)
            {
                return E_ConservativeRaster::L1;
            }
            else if (Model == E_RHIFeatureModel::DESKTOP_0)
            {
                return E_ConservativeRaster::L1;
            }
            return E_ConservativeRaster::NONE;
        }
        
        enum class E_Binding : U8
        {
            NONE = 0x0,

            // Limited descriptors
            // Small resource tables
            L1 = 0x1,
            
            // Large descriptor tables
            // Dynamic resource indexing
            L2 = 0x2,

            // Fully bindless resource model
            // Thousands of descriptors
            L3 = 0x3
        } Binding = E_Binding::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_Binding);
        static E_Binding GetMinimalBinding(E_RHIFeatureModel Model)
        {
            if (Model == E_RHIFeatureModel::DESKTOP_1)
            {
                return E_Binding::L3;
            }
            else if (Model == E_RHIFeatureModel::DESKTOP_0)
            {
                return E_Binding::L2;
            }
            return E_Binding::L1;
        }
        
        enum class E_WaveOps : U8
        {
            NONE = 0x0,

            // Basic wave intrinsics
            // - WaveActiveSum
            // - WaveActiveMax
            L1 = 0x1
        } WaveOps = E_WaveOps::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_WaveOps);
        static E_WaveOps GetMinimalWaveOps(E_RHIFeatureModel Model)
        {
            if (Model == E_RHIFeatureModel::DESKTOP_0)
            {
                return E_WaveOps::L1;
            }
            else if (Model == E_RHIFeatureModel::XR_0)
            {
                return E_WaveOps::L1;
            }
            else if (Model == E_RHIFeatureModel::XR_1)
            {
                return E_WaveOps::L1;
            }
            return E_WaveOps::NONE;
        }
        
        enum class E_AtomicOps : U8
        {
            NONE = 0x0,

            // Basic
            L1 = 0x1,

            // I64 Atomic Ops
            FEATURE_I64 = 0x2
        } AtomicOps = E_AtomicOps::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_AtomicOps);
        static E_AtomicOps GetMinimalAtomicOps(E_RHIFeatureModel Model)
        {
            if (Model == E_RHIFeatureModel::DESKTOP_1)
            {
                return E_AtomicOps::FEATURE_I64;
            }
            else if (Model == E_RHIFeatureModel::DESKTOP_0)
            {
                return E_AtomicOps::L1;
            }
            else if (Model == E_RHIFeatureModel::XR_0)
            {
                return E_AtomicOps::L1;
            }
            else if (Model == E_RHIFeatureModel::XR_1)
            {
                return E_AtomicOps::L1;
            }
            return E_AtomicOps::NONE;
        }
        
        enum class E_PrimitiveTypes : U8
        {
            NONE = 0x0,

            // Basic
            L1 = 0x1,

            // F16 type
            FEATURE_F16 = 0x2
        } PrimitiveTypes = E_PrimitiveTypes::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_PrimitiveTypes);
        static E_PrimitiveTypes GetMinimalPrimitiveTypes(E_RHIFeatureModel Model)
        {
            if (Model == E_RHIFeatureModel::DESKTOP_1)
            {
                return E_PrimitiveTypes::FEATURE_F16;
            }
            return E_PrimitiveTypes::L1;
        }
        
        enum class E_MultiView : U8
        {
            NONE = 0x0,

            // Basic
            L1 = 0x1
        } MultiView = E_MultiView::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_MultiView);
        static E_MultiView GetMinimalMultiView(E_RHIFeatureModel Model)
        {
            if (Model == E_RHIFeatureModel::XR_1)
            {
                return E_MultiView::L1;
            }
            else if (Model == E_RHIFeatureModel::XR_0)
            {
                return E_MultiView::L1;
            }
            return E_MultiView::NONE;
        }
        
        enum class E_UploadBufferDetachCopyPass : U8
        {
            NONE = 0x0,

            SUPPORTED = 0x1
        } UploadBufferDetachCopyPass = E_UploadBufferDetachCopyPass::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_UploadBufferDetachCopyPass);
        
        enum class E_UploadTextureDetachCopyPass : U8
        {
            NONE = 0x0,

            SUPPORTED = 0x1
        } UploadTextureDetachCopyPass = E_UploadTextureDetachCopyPass::NONE;
        ABYTEK_DEFINE_FLAG_OPERATORS_ADVANCED(friend constexpr, E_UploadTextureDetachCopyPass);

        enum class E_FormatUsageType
        {
            NONE,
            BUFFER,
            IA_VERTEX_BUFFER,
            IA_INDEX_BUFFER,
            SO_BUFFER,
            TEXTURE1D,
            TEXTURE2D,
            TEXTURE3D,
            TEXTURECUBE,
            SHADER_LOAD,
            SHADER_SAMPLE,
            SHADER_SAMPLE_COMPARISON,
            SHADER_SAMPLE_MONO_TEXT,
            MIP,
            RENDER_TARGET,
            BLENDABLE,
            DEPTH_STENCIL,
            MULTISAMPLE_RESOLVE,
            DISPLAY,
            CAST_WITHIN_BIT_LAYOUT,
            MULTISAMPLE_RENDERTARGET,
            MULTISAMPLE_LOAD,
            SHADER_GATHER,
            BACK_BUFFER_CAST,
            TYPED_UNORDERED_ACCESS_VIEW,
            SHADER_GATHER_COMPARISON,
            DECODER_OUTPUT,
            VIDEO_PROCESSOR_OUTPUT,
            VIDEO_PROCESSOR_INPUT,
            VIDEO_ENCODER,
            UAV_ATOMIC_ADD,
            UAV_ATOMIC_BITWISE_OPS,
            UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE,
            UAV_ATOMIC_EXCHANGE,
            UAV_ATOMIC_SIGNED_MIN_OR_MAX,
            UAV_ATOMIC_UNSIGNED_MIN_OR_MAX,
            UAV_TYPED_LOAD,
            UAV_TYPED_STORE,
            OUTPUT_MERGER_LOGIC_OP,
            TILED,
            MULTIPLANE_OVERLAY,
            SAMPLER_FEEDBACK,
            
            NUM
        };
        struct F_FormatUsage
        {
            E_FormatUsageType Type = E_FormatUsageType::NONE;
            TF_Set<E_RHIFormat> Formats;
        
            friend B8 operator == (const F_FormatUsage& A, const F_FormatUsage& B)
            {
                return (
                    (A.Type == B.Type)    
                    && (A.Formats == B.Formats)
                );
            }
        
            friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_FormatUsage& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Type);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Formats);
                return F_FeedbackStatus::MakeSucceeded();
            }
            friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_FormatUsage& Value) noexcept
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Type);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Formats);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        TF_Vector<F_FormatUsage> FormatUsages;
        
        TF_Vector<E_RHIFeatureModel> GetModels() const
        {
            TF_Vector<E_RHIFeatureModel> Models;

            auto SupportsDesktop0 = [&]()
            {
                return
                    Binding >= E_Binding::L1 &&
                    PrimitiveTypes >= E_PrimitiveTypes::L1 &&
                    AtomicOps >= E_AtomicOps::L1;
            };

            auto SupportsDesktop1 = [&]()
            {
                return
                    SupportsDesktop0() &&
                    WaveOps >= E_WaveOps::L1 &&
                    Binding >= E_Binding::L2 &&
                    (
                        MeshShader >= E_MeshShader::L1 ||
                        RayTracing >= E_RayTracing::L1
                    );
            };

            auto SupportsXR0 = [&]()
            {
                return
                    Binding >= E_Binding::L1 &&
                    MultiView >= E_MultiView::L1 &&
                    PrimitiveTypes >= E_PrimitiveTypes::L1;
            };

            auto SupportsXR1 = [&]()
            {
                return
                    SupportsXR0() &&
                    VariableRateShading >= E_VariableRateShading::L1 &&
                    Binding >= E_Binding::L2 &&
                    WaveOps >= E_WaveOps::L1;
            };

            if (SupportsDesktop0())
                Models.push_back(E_RHIFeatureModel::DESKTOP_0);

            if (SupportsDesktop1())
                Models.push_back(E_RHIFeatureModel::DESKTOP_1);

            if (SupportsXR0())
                Models.push_back(E_RHIFeatureModel::XR_0);

            if (SupportsXR1())
                Models.push_back(E_RHIFeatureModel::XR_1);
            
            Models.push_back(E_RHIFeatureModel::MOBILE_0);

            return ABYTEK_MOVE(Models);
        }
        
        friend B8 operator == (const F_RHIFeatureSet& A, const F_RHIFeatureSet& B)
        {
            return (
                (A.RayTracing == B.RayTracing)    
                && (A.MeshShader == B.MeshShader)    
                && (A.VariableRateShading == B.VariableRateShading)    
                && (A.SamplerFeedback == B.SamplerFeedback)    
                && (A.ConservativeRaster == B.ConservativeRaster)    
                && (A.Binding == B.Binding)    
                && (A.WaveOps == B.WaveOps)    
                && (A.AtomicOps == B.AtomicOps)    
                && (A.UploadBufferDetachCopyPass == B.UploadBufferDetachCopyPass)    
                && (A.UploadTextureDetachCopyPass == B.UploadTextureDetachCopyPass)    
                && (A.PrimitiveTypes == B.PrimitiveTypes)    
                && (A.MultiView == B.MultiView)    
                && (A.FormatUsages == B.FormatUsages)    
            );
        }
        friend B8 operator != (const F_RHIFeatureSet& A, const F_RHIFeatureSet& B)
        {
            return !(A == B);
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIFeatureSet& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RayTracing);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MeshShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.VariableRateShading);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SamplerFeedback);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ConservativeRaster);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Binding);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.WaveOps);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.AtomicOps);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UploadBufferDetachCopyPass);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UploadTextureDetachCopyPass);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PrimitiveTypes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MultiView);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.FormatUsages);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIFeatureSet& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RayTracing);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MeshShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.VariableRateShading);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SamplerFeedback);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ConservativeRaster);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Binding);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.WaveOps);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.AtomicOps);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.UploadBufferDetachCopyPass);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.UploadTextureDetachCopyPass);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PrimitiveTypes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MultiView);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.FormatUsages);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        template<typename __F>
        static B8 Check(__F Desire, __F Minimal, __F Specified = Minimal)
        {
            return (
                (Minimal >= Desire)
                || (Specified >= Desire)
            );
        }
    };

    struct F_RHIFeatureSupports
    {
        E_RHIFeatureModel Model = E_RHIFeatureModel::DEFAULT;
        F_RHIFeatureSet ExtendedSet;
        
        friend B8 operator == (const F_RHIFeatureSupports& A, const F_RHIFeatureSupports& B)
        {
            return (
                (A.Model == B.Model)    
                && (A.ExtendedSet == B.ExtendedSet)
            );
        }
        friend B8 operator != (const F_RHIFeatureSupports& A, const F_RHIFeatureSupports& B)
        {
            return !(A == B);
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIFeatureSupports& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Model);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ExtendedSet);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIFeatureSupports& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Model);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ExtendedSet);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
}

#define ABYTEK_RHI_FEATURE_CHECK(Name, Desire, ...) \
    Abytek::F_RHIFeatureSet::Check( \
        Desire, \
        ABYTEK_GLUE(F_RHIFeatureSet::GetMinimal, Name)( \
            __VA_ARGS__.Model \
        ), \
        __VA_ARGS__.ABYTEK_GLUE(ExtendedSet., Name) \
    )