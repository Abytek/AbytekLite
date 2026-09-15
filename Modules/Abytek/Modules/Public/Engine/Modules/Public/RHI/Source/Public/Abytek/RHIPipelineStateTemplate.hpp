#pragma once

#include "Abytek/RHITemplate.hpp"
#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIFeature.hpp"
#include "Abytek/RHIPipelineState.hpp"
#include "Abytek/RHIBindGroupTemplate.hpp"
#include "Abytek/RHIPrimitiveTopology.hpp"
#include "Abytek/RHIRasterizerConfig.hpp"
#include "Abytek/RHIDepthStencilConfig.hpp"
#include "Abytek/RHIBlendConfig.hpp"


namespace Abytek
{
    class A_RHITemplateDatabase;
    class A_RHIBindGroupTemplate;
    
    struct F_RHIPipelineStateTemplateBindGroup
    {
        F_RHITemplateHashCode TemplateHashCode = INVALID_RHI_TEMPLATE_HASH_CODE;

        static F_RHIPipelineStateTemplateBindGroup Make(const TS_Valid<A_RHIBindGroupTemplate>& InBindGroupTemplate)
        {
            F_RHIPipelineStateTemplateBindGroup Result;
            Result.TemplateHashCode = InBindGroupTemplate->GetHashCode();
            return Result;
        }
        static F_RHIPipelineStateTemplateBindGroup Make(F_RHITemplateHashCode InTemplateHashCode)
        {
            F_RHIPipelineStateTemplateBindGroup Result;
            Result.TemplateHashCode = InTemplateHashCode;
            return Result;
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIPipelineStateTemplateBindGroup& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.TemplateHashCode);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIPipelineStateTemplateBindGroup& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.TemplateHashCode);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIPipelineStateTemplateBindGroup& A, const F_RHIPipelineStateTemplateBindGroup& B)
        {
            return (
                (A.TemplateHashCode == B.TemplateHashCode)
            );
        }
        friend B8 operator != (const F_RHIPipelineStateTemplateBindGroup& A, const F_RHIPipelineStateTemplateBindGroup& B)
        {
            return !(A == B);
        }
    };

    enum class E_RHIShaderSourceType : U8
    {
        NONE,
        SLANG,
        DEFAULT = SLANG
    };
    struct F_RHIShaderSourceSlang
    {
        F_Name EntryPointName;
        TF_Set<F_Name> ModuleNames;
        TF_Set<F_Text> SearchPaths;
        TF_Map<F_Name, F_Text> Definitions;

        static F_RHIShaderSourceSlang Make(
            const F_Name& InEntryPointName,
            const TF_Set<F_Name>& InModuleNames,
            const TF_Set<F_Text>& InSearchPaths,
            const TF_Map<F_Name, F_Text>& InDefinitions = {}
        )
        {
            F_RHIShaderSourceSlang Result;
            Result.EntryPointName = InEntryPointName;
            Result.ModuleNames = InModuleNames;
            Result.SearchPaths = InSearchPaths;
            Result.Definitions = InDefinitions;
            return ABYTEK_MOVE(Result);
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIShaderSourceSlang& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.EntryPointName);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ModuleNames);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SearchPaths);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Definitions);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIShaderSourceSlang& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.EntryPointName);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ModuleNames);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SearchPaths);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Definitions);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIShaderSourceSlang& A, const F_RHIShaderSourceSlang& B)
        {
            return (
                (A.EntryPointName == B.EntryPointName)    
                && (A.ModuleNames == B.ModuleNames)    
                && (A.SearchPaths == B.SearchPaths)    
                && (A.Definitions == B.Definitions)    
            );
        }
        friend B8 operator != (const F_RHIShaderSourceSlang& A, const F_RHIShaderSourceSlang& B)
        {
            return !(A == B);
        }
        
        template<typename __F_Callback>
        void ForEachModuleFile(__F_Callback&& Callback) const
        {
            for (const auto& ModuleName : ModuleNames)
            {
                TF_Optional<F_Text> FilePath;
                for (const auto& SearchPath_ : SearchPaths)
                {
                    F_Text SearchFilePath = SearchPath_ + ABYTEK_TEXT("/") + *ModuleName + ABYTEK_TEXT(".slang");
                    if (H_FSUtilities::Exists(SearchFilePath, E_FSEntryType::FILE))
                    {
                        FilePath = SearchFilePath;
                        break;
                    }
                }
                Callback(ModuleName, FilePath);
            }
        }
    };
    struct F_RHIShaderSource
    {
        E_RHIShaderSourceType Type = E_RHIShaderSourceType::DEFAULT;

        F_RHIShaderSourceSlang Slang;

        static F_RHIShaderSource Make(const F_RHIShaderSourceSlang& InSlang)
        {
            F_RHIShaderSource Result;
            Result.Type = E_RHIShaderSourceType::SLANG;
            Result.Slang = InSlang;
            return ABYTEK_MOVE(Result);
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIShaderSource& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Type);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Slang);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIShaderSource& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Type);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Slang);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIShaderSource& A, const F_RHIShaderSource& B)
        {
            if (A.Type != B.Type)
            {
                return false;
            }
            switch (A.Type)
            {
            case E_RHIShaderSourceType::NONE:
                break;
            case E_RHIShaderSourceType::SLANG:
                if (A.Slang != B.Slang)
                {
                    return false;
                }
                break;
            default:
                ABYTEK_LOG_FATAL() << "Invalid shader source type: " << static_cast<U32>(A.Type);
                break;
            }
            return true;
        }
        friend B8 operator != (const F_RHIShaderSource& A, const F_RHIShaderSource& B)
        {
            return !(A == B);
        }
    };
    
    struct F_RHISlangShaderFileVersion
    {
        F_Text Path;
        U64 Hash = 0;
        
        static F_RHISlangShaderFileVersion Make(const F_Text& InPath)
        {
            F_RHISlangShaderFileVersion Result;
            Result.Path = H_Path::Normalize(InPath);
            return ABYTEK_MOVE(Result);
        }
        F_FeedbackStatus LoadCurrent()
        {
            F_Text Content;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                H_FSUtilities::ReadFileText(Path, Content)
            );
            Hash = 0;
            boost::hash_combine(Hash, Content);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHISlangShaderFileVersion& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Path);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Hash);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHISlangShaderFileVersion& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Path);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Hash);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };

    enum class E_RHIShaderTargetType : U8
    {
        NONE,
        DXIL,
        HLSL
    };
    struct F_RHIShaderTargetDXIL
    {
        TF_Vector<U8> Binary;
        
        static F_RHIShaderTargetDXIL Make(
            const TF_Vector<U8>& InBinary
        )
        {
            F_RHIShaderTargetDXIL Result;
            Result.Binary = InBinary;
            return ABYTEK_MOVE(Result);
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIShaderTargetDXIL& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Binary);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIShaderTargetDXIL& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Binary);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    struct F_RHIShaderTargetHLSL
    {
        F_Text Text;
        
        static F_RHIShaderTargetHLSL Make(
            const F_Text& InText
        )
        {
            F_RHIShaderTargetHLSL Result;
            Result.Text = InText;
            return ABYTEK_MOVE(Result);
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIShaderTargetHLSL& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Text);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIShaderTargetHLSL& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Text);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    struct F_RHIShaderTarget
    {
        E_RHIShaderTargetType Type = E_RHIShaderTargetType::NONE;
        F_RHIShaderTargetDXIL DXIL;
        F_RHIShaderTargetHLSL HLSL;

        static F_RHIShaderTarget Make(const F_RHIShaderTargetDXIL& InDXL)
        {
            F_RHIShaderTarget Result;
            Result.Type = E_RHIShaderTargetType::DXIL;
            Result.DXIL = InDXL;
            return ABYTEK_MOVE(Result);
        }
        static F_RHIShaderTarget Make(const F_RHIShaderTargetHLSL& InHLSL)
        {
            F_RHIShaderTarget Result;
            Result.Type = E_RHIShaderTargetType::HLSL;
            Result.HLSL = InHLSL;
            return ABYTEK_MOVE(Result);
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIShaderTarget& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Type);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DXIL);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.HLSL);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIShaderTarget& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Type);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DXIL);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.HLSL);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };

    struct F_RHIPipelineStateTemplateConfig
    {
        E_RHIPipelineStateType Type = E_RHIPipelineStateType::NONE;
        TF_Vector<F_RHIPipelineStateTemplateBindGroup> BindGroups;
        B8 AllowBindless = false;
        
        E_RHIPrimitiveTopology PrimitiveTopology = E_RHIPrimitiveTopology::DEFAULT;
        F_RHIRasterizerConfig Rasterizer;
        F_RHIDepthStencilConfig DepthStencil;
        F_RHIBlendConfig Blend;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIPipelineStateTemplateConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Type);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.BindGroups);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.AllowBindless);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PrimitiveTopology);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Rasterizer);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DepthStencil);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Blend);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIPipelineStateTemplateConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Type);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.BindGroups);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.AllowBindless);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PrimitiveTopology);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Rasterizer);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DepthStencil);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Blend);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIPipelineStateTemplateConfig& A, const F_RHIPipelineStateTemplateConfig& B)
        {
            return (
                (A.Type == B.Type)
                && (A.BindGroups == B.BindGroups)
                && (A.AllowBindless == B.AllowBindless)
                && (A.PrimitiveTopology == B.PrimitiveTopology)
                && (A.Rasterizer == B.Rasterizer)
                && (A.DepthStencil == B.DepthStencil)
                && (A.Blend == B.Blend)
            );
        }
        friend B8 operator != (const F_RHIPipelineStateTemplateConfig& A, const F_RHIPipelineStateTemplateConfig& B)
        {
            return !(A == B);
        }
    };
    struct F_RHIPipelineStateTemplateBuildParams : F_RHITemplateBuildParamsBase, F_RHIPipelineStateTemplateConfig
    {
    };

    struct F_RHIPipelineStateTemplateShader
    {
        F_RHIShaderSource Source;

    protected:
        static F_RHIPipelineStateTemplateShader Make(const F_RHIShaderSource& InSource)
        {
            F_RHIPipelineStateTemplateShader Result;
            Result.Source = InSource;
            return ABYTEK_MOVE(Result);
        }
        
    public:
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIPipelineStateTemplateShader& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Source);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIPipelineStateTemplateShader& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Source);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIPipelineStateTemplateShader& A, const F_RHIPipelineStateTemplateShader& B)
        {
            return (
                (A.Source == B.Source)
            );
        }
        friend B8 operator != (const F_RHIPipelineStateTemplateShader& A, const F_RHIPipelineStateTemplateShader& B)
        {
            return !(A == B);
        }
    };
    struct F_RHIPipelineStateTemplateVertexShader : F_RHIPipelineStateTemplateShader
    {
    public:
        static F_RHIPipelineStateTemplateVertexShader Make(const F_RHIShaderSource& InSource)
        {
            F_RHIPipelineStateTemplateVertexShader Result = {
                F_RHIPipelineStateTemplateShader::Make(InSource)
            };
            return ABYTEK_MOVE(Result);
        }
    };
    struct F_RHIPipelineStateTemplatePixelShader : F_RHIPipelineStateTemplateShader
    {
    public:
        static F_RHIPipelineStateTemplatePixelShader Make(const F_RHIShaderSource& InSource)
        {
            F_RHIPipelineStateTemplatePixelShader Result = {
                F_RHIPipelineStateTemplateShader::Make(InSource)
            };
            return ABYTEK_MOVE(Result);
        }
    };
    struct F_RHIPipelineStateTemplateMeshShader : F_RHIPipelineStateTemplateShader
    {
    public:
        static F_RHIPipelineStateTemplateMeshShader Make(const F_RHIShaderSource& InSource)
        {
            F_RHIPipelineStateTemplateMeshShader Result = {
                F_RHIPipelineStateTemplateShader::Make(InSource)
            };
            return ABYTEK_MOVE(Result);
        }
    };
    struct F_RHIPipelineStateTemplateAmplificationShader : F_RHIPipelineStateTemplateShader
    {
    public:
        static F_RHIPipelineStateTemplateAmplificationShader Make(const F_RHIShaderSource& InSource)
        {
            F_RHIPipelineStateTemplateAmplificationShader Result = {
                F_RHIPipelineStateTemplateShader::Make(InSource)
            };
            return ABYTEK_MOVE(Result);
        }
    };
    struct F_RHIPipelineStateTemplateComputeShader : F_RHIPipelineStateTemplateShader
    {
    public:
        static F_RHIPipelineStateTemplateComputeShader Make(const F_RHIShaderSource& InSource)
        {
            F_RHIPipelineStateTemplateComputeShader Result = {
                F_RHIPipelineStateTemplateShader::Make(InSource)
            };
            return ABYTEK_MOVE(Result);
        }
    };
    
    namespace Internal
    {
        template<E_RHIShaderFrequency __ShaderFrequency>
        struct TH_RHIPipelineStateTemplateShader;
        
        template<>
        struct TH_RHIPipelineStateTemplateShader<E_RHIShaderFrequency::VERTEX>
        {
            using F = F_RHIPipelineStateTemplateVertexShader;
        };
        template<>
        struct TH_RHIPipelineStateTemplateShader<E_RHIShaderFrequency::PIXEL>
        {
            using F = F_RHIPipelineStateTemplatePixelShader;
        };
        template<>
        struct TH_RHIPipelineStateTemplateShader<E_RHIShaderFrequency::MESH>
        {
            using F = F_RHIPipelineStateTemplateMeshShader;
        };
        template<>
        struct TH_RHIPipelineStateTemplateShader<E_RHIShaderFrequency::AMPLIFICATION>
        {
            using F = F_RHIPipelineStateTemplateAmplificationShader;
        };
        template<>
        struct TH_RHIPipelineStateTemplateShader<E_RHIShaderFrequency::COMPUTE>
        {
            using F = F_RHIPipelineStateTemplateComputeShader;
        };
    }
    template<E_RHIShaderFrequency __ShaderFrequency>
    using TF_RHIPipelineStateTemplateShader = typename Internal::TH_RHIPipelineStateTemplateShader<__ShaderFrequency>::F;
    
    struct F_RHIPipelineStateTemplateCompiledShader
    {
        TF_Vector<F_RHIShaderTarget> Targets;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIPipelineStateTemplateCompiledShader& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Targets);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIPipelineStateTemplateCompiledShader& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Targets);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    struct F_RHIPipelineStateTemplateCompiledVertexShader : F_RHIPipelineStateTemplateCompiledShader
    {
    };
    struct F_RHIPipelineStateTemplateCompiledPixelShader : F_RHIPipelineStateTemplateCompiledShader
    {
    };
    struct F_RHIPipelineStateTemplateCompiledMeshShader : F_RHIPipelineStateTemplateCompiledShader
    {
    };
    struct F_RHIPipelineStateTemplateCompiledAmplificationShader : F_RHIPipelineStateTemplateCompiledShader
    {
    };
    struct F_RHIPipelineStateTemplateCompiledComputeShader : F_RHIPipelineStateTemplateCompiledShader
    {
    };
    
    struct F_RHIPipelineStateTemplateCompileConfig
    {
        TF_Optional<F_RHIPipelineStateTemplateVertexShader> VertexShader;
        TF_Optional<F_RHIPipelineStateTemplatePixelShader> PixelShader;
        TF_Optional<F_RHIPipelineStateTemplateMeshShader> MeshShader;
        TF_Optional<F_RHIPipelineStateTemplateAmplificationShader> AmplificationShader;
        TF_Optional<F_RHIPipelineStateTemplateComputeShader> ComputeShader;
        B8 DebugGeneratedShaders = false;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIPipelineStateTemplateCompileConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.VertexShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PixelShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MeshShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.AmplificationShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ComputeShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DebugGeneratedShaders);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIPipelineStateTemplateCompileConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.VertexShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PixelShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MeshShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.AmplificationShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ComputeShader);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DebugGeneratedShaders);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIPipelineStateTemplateCompileConfig& A, const F_RHIPipelineStateTemplateCompileConfig& B)
        {
            return (
                (A.VertexShader == B.VertexShader)
                && (A.PixelShader == B.PixelShader)
                && (A.MeshShader == B.MeshShader)
                && (A.AmplificationShader == B.AmplificationShader)
                && (A.ComputeShader == B.ComputeShader)
                && (A.DebugGeneratedShaders == B.DebugGeneratedShaders)
            );
        }
        friend B8 operator != (const F_RHIPipelineStateTemplateCompileConfig& A, const F_RHIPipelineStateTemplateCompileConfig& B)
        {
            return !(A == B);
        }
        
        template<typename __F_Callback, E_RHIShaderFrequency __ShaderFrequencies = E_RHIShaderFrequency::ALL>
        void ForEachShaderSource(__F_Callback&& Callback, E_RHIShaderFrequency ShaderFrequencies = __ShaderFrequencies) const
        {
            if (VertexShader && FlagHas(ShaderFrequencies, E_RHIShaderFrequency::VERTEX))
            {
                Callback(VertexShader->Source);
            }
            if (PixelShader && FlagHas(ShaderFrequencies, E_RHIShaderFrequency::PIXEL))
            {
                Callback(PixelShader->Source);
            }
            if (MeshShader && FlagHas(ShaderFrequencies, E_RHIShaderFrequency::MESH))
            {
                Callback(MeshShader->Source);
            }
            if (AmplificationShader && FlagHas(ShaderFrequencies, E_RHIShaderFrequency::AMPLIFICATION))
            {
                Callback(AmplificationShader->Source);
            }
            if (ComputeShader && FlagHas(ShaderFrequencies, E_RHIShaderFrequency::COMPUTE))
            {
                Callback(ComputeShader->Source);
            }
        }
        template<typename __F_Callback, E_RHIShaderFrequency __ShaderFrequencies = E_RHIShaderFrequency::ALL>
        void ForEachShaderSource(__F_Callback&& Callback, E_RHIShaderFrequency ShaderFrequencies = __ShaderFrequencies)
        {
            if (VertexShader)
            {
                Callback(VertexShader->Source);
            }
            if (PixelShader)
            {
                Callback(PixelShader->Source);
            }
            if (MeshShader)
            {
                Callback(MeshShader->Source);
            }
            if (AmplificationShader)
            {
                Callback(AmplificationShader->Source);
            }
            if (ComputeShader)
            {
                Callback(ComputeShader->Source);
            }
        }
        
        template<E_RHIShaderFrequency __ShaderFrequencies = E_RHIShaderFrequency::ALL>
        F_FeedbackStatus AddShaderDefinition(const F_Name& Key, const F_Text& Value = ABYTEK_TEXT("1"), E_RHIShaderFrequency ShaderFrequencies = __ShaderFrequencies)
        {
            F_FeedbackStatus FeedbackStatus = F_FeedbackStatus::MakeSucceeded();
            ForEachShaderSource(
                [&FeedbackStatus, &Key, &Value](F_RHIShaderSource& ShaderSource)
                {
                    if (!FeedbackStatus)
                    {
                        return;
                    }
                    ABYTEK_ENGINE_NFC_ASSERT(ShaderSource.Type == E_RHIShaderSourceType::SLANG) << "Non-slang shader sources are not supported";
                    if (ShaderSource.Slang.Definitions.find(Key) != ShaderSource.Slang.Definitions.end())
                    {
                        FeedbackStatus = F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Already added shader definition: ") + *Key);
                        return;
                    }
                    ShaderSource.Slang.Definitions[Key] = Value;
                },
                ShaderFrequencies
            );
            return FeedbackStatus;
        }
        template<E_RHIShaderFrequency __ShaderFrequencies = E_RHIShaderFrequency::ALL>
        F_FeedbackStatus SetShaderDefinition(const F_Name& Key, const F_Text& Value = ABYTEK_TEXT("1"), E_RHIShaderFrequency ShaderFrequencies = __ShaderFrequencies)
        {
            ForEachShaderSource(
                [&Key, &Value](F_RHIShaderSource& ShaderSource)
                {
                    ABYTEK_ENGINE_NFC_ASSERT(ShaderSource.Type == E_RHIShaderSourceType::SLANG) << "Non-slang shader sources are not supported";
                    ShaderSource.Slang.Definitions[Key] = Value;
                },
                ShaderFrequencies
            );
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    struct F_RHIPipelineStateTemplateCompileParams : F_RHIPipelineStateTemplateBuildParams, F_RHIPipelineStateTemplateCompileConfig
    {
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIPipelineStateTemplate : public A_RHITemplate
    {
    private:
        F_RHIPipelineStateTemplateConfig _Config;
        TF_Vector<TS<A_RHIBindGroupTemplate>> _BindGroupTemplates;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        F_RHIPipelineStateTemplateCompileConfig _CompileConfig;
        TF_Map<F_Text, F_RHISlangShaderFileVersion> _SlangShaderFileVersions;
#endif

    public:
        ABYTEK_FORCE_INLINE auto GetConfig() const noexcept
        {
            return _Config;
        }
        ABYTEK_FORCE_INLINE const auto& GetBindGroupTemplates() const noexcept
        {
            return _BindGroupTemplates;
        }
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        ABYTEK_FORCE_INLINE const auto& GetCompileConfig() const noexcept
        {
            return _CompileConfig;
        }
        ABYTEK_FORCE_INLINE const auto& GetSlangShaderFileVersions() const noexcept
        {
            return _SlangShaderFileVersions;
        }
#endif
        
        ABYTEK_FORCE_INLINE auto GetType() const noexcept
        {
            return _Config.Type;
        }
        ABYTEK_FORCE_INLINE const auto& GetBindGroups() const noexcept
        {
            return _Config.BindGroups;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetPrimitiveTopology() const noexcept
        {
            return _Config.PrimitiveTopology;
        }
        ABYTEK_FORCE_INLINE const auto& GetRasterizer() const noexcept
        {
            return _Config.Rasterizer;
        }
        ABYTEK_FORCE_INLINE const auto& GetDepthStencil() const noexcept
        {
            return _Config.DepthStencil;
        }
        ABYTEK_FORCE_INLINE const auto& GetBlend() const noexcept
        {
            return _Config.Blend;
        }

    protected:
        A_RHIPipelineStateTemplate(const F_RHIPipelineStateTemplateBuildParams& BuildParams);

    public:
        ~A_RHIPipelineStateTemplate() override;

    protected:
        TS_Valid<A_RHITemplateRuntime> CreateAndBuildRuntime(const TW_Valid<A_RHIContext>& Context) override;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    public:
        void SetCompileConfig(const F_RHIPipelineStateTemplateCompileConfig& X)
        {
            _CompileConfig = X;
        }
        void AddSlangShaderFileVersion(const F_RHISlangShaderFileVersion& X)
        {
            _SlangShaderFileVersions.insert({ X.Path, X });
        }
#endif
    };
}