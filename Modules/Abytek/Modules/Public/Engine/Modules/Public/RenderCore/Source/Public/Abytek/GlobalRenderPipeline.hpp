#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/RenderRegistryRuntime.hpp"
#include "Abytek/Permutation.hpp"
#include "Abytek/RenderPack.hpp"
#include "Abytek/ResourceUtilities.hpp"
#include "Abytek/RHIPipelineStateTemplate.hpp"
#include "Abytek/RenderCoreHelper.hpp"


namespace Abytek
{
    class F_GlobalRenderPack;
    
    namespace Internal::GlobalRenderPipeline
    {
#ifdef ABYTEK_ENABLE_INTERNAL_GLOBAL_RENDER_PIPELINE_DEBUG
        static constexpr B8 EnableInternalDebugger = true;
#else
        static constexpr B8 EnableInternalDebugger = false;
#endif
    }
    
    template<typename __F_DynamicPermutationVector>
    struct TF_GlobalRenderPipelineConfig : F_RHIPipelineStateTemplateCompileParams
    {
        B8 ShouldCompile = true;
        TS<F_RenderPack> Pack;
        __F_DynamicPermutationVector PermutationVector;
        F_RHITemplateHashCode PermutationHashCode;
    };

    struct F_GlobalRenderPipeline
    {
        using F_Metadata_BuildCommandsAndCompilationSet = TF_Function<
            F_FeedbackStatus(
                const TS<F_RenderPack>& Pack, 
                const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
                const TW_Valid<A_RenderPackTemplateMap>& TemplateMap,
                TF_Vector<TF_Function<F_FeedbackStatus(TF_Vector<TS<A_RHITemplate>>& OutTemplates)>>& OutCommands, 
                TF_Set<F_RHITemplateHashCode>& OutTemplateHashCodesToCompile
            )
        >;
        static F_Name GetMetadataElementName_BuildCommandsAndCompilationSet()
        {
            return ABYTEK_NAME("Abytek::F_GlobalRenderPipeline::BuildCommandsAndCompilationSet");
        }
        
        ABYTEK_BEGIN_REFLECTOR();
            ABYTEK_REFLECT_GEN_INTERFACE()
            {
                auto& Metadata = ReflectionType->GetMetadata();
                Metadata.Add( 
                    GetMetadataElementName_BuildCommandsAndCompilationSet(),
                    F_Metadata_BuildCommandsAndCompilationSet(
                        &__F_SelfOrInherited::BuildCommandsAndCompilationSet
                    )
                );
            }
        ABYTEK_END_REFLECTOR(F_GlobalRenderPipeline);
        
        using F_PermutationDomain = F_DefaultPermutationDomain;
        
        F_RHITemplateHashCode TemplateHashCode = INVALID_RHI_TEMPLATE_HASH_CODE;
        F_RHITemplateHashCode PermutationHashCode = 0;
        TS<A_RHIPipelineStateTemplateRuntime> TemplateRuntime;

        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return TemplateRuntime.IsValid() && (TemplateHashCode != INVALID_RHI_TEMPLATE_HASH_CODE);
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        TS_Valid<A_RHIPipelineState> AcquirePipelineState() const
        {
            return TemplateRuntime;
        }
        void Reset()
        {
            TemplateHashCode = 0;
            PermutationHashCode = 0;
            TemplateRuntime.Reset();
        }
        friend B8 operator == (const F_GlobalRenderPipeline& A, const F_GlobalRenderPipeline& B) noexcept
        {
            return (
                (A.TemplateHashCode == B.TemplateHashCode)    
                && (A.PermutationHashCode == B.PermutationHashCode)    
                && (A.TemplateRuntime == B.TemplateRuntime)    
            );
        }
        friend B8 operator != (const F_GlobalRenderPipeline& A, const F_GlobalRenderPipeline& B) noexcept
        {
            return (
                (A.TemplateHashCode != B.TemplateHashCode)    
                || (A.PermutationHashCode != B.PermutationHashCode)    
                || (A.TemplateRuntime != B.TemplateRuntime)    
            );
        }
    };
    
    using F_ShaderDefinitionSet = TF_Vector<std::pair<F_Name, F_Text>>;
}

#define ABYTEK_GLOBAL_RENDER_PIPELINE(Name, ...) \
            ABYTEK_BEGIN_REFLECTOR(Abytek::F_GlobalRenderPipeline) \
            ABYTEK_END_REFLECTOR(Name) \
            { \
                ABYTEK_REFLECT_CANONICAL(__VA_ARGS__); \
            }; \
             \
            using F_DynamicPermutationVector = typename F_PermutationDomain::F_DynamicVector; \
            using F_DefaultStaticPermutationVector = typename F_PermutationDomain::F_DefaultStaticVector; \
            using F_Config = Abytek::TF_GlobalRenderPipelineConfig<F_DynamicPermutationVector>; \
            static constexpr Abytek::F_RHITemplateHashCode DefaultPermutationHashCode = F_DefaultStaticPermutationVector::GetHashCode(); \
             \
            static constexpr Abytek::F_RHITemplateHashCode GetTemplateHashCode(Abytek::F_RHITemplateHashCode InPermutationHashCode = DefaultPermutationHashCode) \
            { \
                Abytek::F_RHITemplateHashCode UseDefinedHashCode = 0; \
                UseDefinedHashCode = Abytek::HashCombineU64(UseDefinedHashCode, Abytek::TypeHashCode<Name>); \
                UseDefinedHashCode = Abytek::HashCombineU64(UseDefinedHashCode, InPermutationHashCode); \
                return Abytek::H_RenderCore::GenerateTemplateHashCode<F_GlobalRenderPack>(UseDefinedHashCode); \
            } \
            static constexpr Abytek::F_RHITemplateHashCode GetTemplateHashCode( \
                const F_DynamicPermutationVector& PermutationVector \
            ) \
            { \
                return GetTemplateHashCode(PermutationVector.GetHashCode()); \
            } \
            template<typename __F_StaticPermutationVector> \
            static constexpr Abytek::F_RHITemplateHashCode GetTemplateHashCode() \
            { \
                return GetTemplateHashCode(__F_StaticPermutationVector::GetHashCode()); \
            } \
            B8 Update( \
                const Abytek::TS_Valid<Abytek::F_RenderRegistryRuntime>& RegistryRuntime, \
                Abytek::F_RHITemplateHashCode InPermutationHashCode = DefaultPermutationHashCode \
            ) \
            { \
                if (IsValid() && (InPermutationHashCode == PermutationHashCode)) \
                { \
                    return true; \
                } \
                auto NewTemplateHashCode = GetTemplateHashCode(InPermutationHashCode); \
                auto NewTemplateRuntime = RegistryRuntime->GetOrActivate(NewTemplateHashCode); \
                if (!NewTemplateRuntime) \
                { \
                    return false; \
                } \
                TemplateRuntime = NewTemplateRuntime.FastCast<Abytek::A_RHIPipelineStateTemplateRuntime>(); \
                TemplateHashCode = NewTemplateHashCode; \
                PermutationHashCode = InPermutationHashCode; \
                return true; \
            } \
            B8 Update( \
                const Abytek::TS_Valid<Abytek::F_RenderRegistryRuntime>& RegistryRuntime, \
                const F_DynamicPermutationVector& PermutationVector \
            ) \
            { \
                return Update(RegistryRuntime, PermutationVector.GetHashCode()); \
            } \
            template<typename __F_StaticPermutationVector> \
            B8 Update( \
                const Abytek::TS_Valid<Abytek::F_RenderRegistryRuntime>& RegistryRuntime \
            ) \
            { \
                return Update(RegistryRuntime, __F_StaticPermutationVector::GetHashCode()); \
            } \
            static Name Instantiate( \
                const Abytek::TS_Valid<Abytek::F_RenderRegistryRuntime>& RegistryRuntime, \
                Abytek::F_RHITemplateHashCode InPermutationHashCode = DefaultPermutationHashCode \
            ) \
            { \
                Name Result; \
                Result.Update(RegistryRuntime, InPermutationHashCode); \
                ABYTEK_ENGINE_RENDER_CORE_ASSERT(Result.TemplateRuntime) << "Failed to instantiate global render pipeline: " << Abytek::TypeFullName<Name>(); \
                return ABYTEK_MOVE(Result); \
            } \
            static Name Instantiate( \
                const Abytek::TS_Valid<Abytek::F_RenderRegistryRuntime>& RegistryRuntime, \
                const F_DynamicPermutationVector& PermutationVector \
            ) \
            { \
                return Instantiate(RegistryRuntime, PermutationVector.GetHashCode()); \
            } \
            template<typename __F_StaticPermutationVector> \
            static Name Instantiate( \
                const Abytek::TS_Valid<Abytek::F_RenderRegistryRuntime>& RegistryRuntime \
            ) \
            { \
                return Instantiate(RegistryRuntime, __F_StaticPermutationVector::GetHashCode()); \
            } \
            static Abytek::TS<Abytek::A_RHITemplate> QueryTemplate( \
                const Abytek::TS_Valid<Abytek::F_RenderRegistry>& Registry, \
                Abytek::F_RHITemplateHashCode InPermutationHashCode = DefaultPermutationHashCode \
            ) \
            { \
                return Registry->QueryTemplate(GetTemplateHashCode(InPermutationHashCode));\
            } \
             \
            ABYTEK_ENABLE_IF_DEVELOPMENT_BUILD( \
                static Abytek::F_FeedbackStatus BuildConfigs( \
                    const Abytek::TS<Abytek::F_RenderPack>& Pack, \
                    const Abytek::TW_Valid<Abytek::F_SerializableEnvironment>& SerializableEnvironment, \
                    const Abytek::TW_Valid<Abytek::A_RenderPackTemplateMap>& TemplateMap, \
                    Abytek::TF_Vector<F_Config>& OutConfigs \
                ) \
                { \
                    auto Registry = Abytek::F_RenderRegistry::GetSerializableEnvironmentMetadataElement_Registry(SerializableEnvironment); \
                    auto Compiler = Registry->GetCompiler(); \
                    auto TemplateDatabase = Registry->GetTemplateDatabase(); \
                     \
                    ABYTEK_LOG_INFO() << "Building configs for global render pipeline: " << Abytek::TypeFullName<Name>(); \
                     \
                    auto PermutationValueSets = F_PermutationDomain::GetValueSets(); \
                    for (const auto& PermutationValueSet : PermutationValueSets) \
                    { \
                        F_Config Config; \
                        Config.Database = TemplateDatabase.Weak(); \
                        Config.PermutationVector = F_DynamicPermutationVector::FromValueSet(PermutationValueSet); \
                        Config.PermutationHashCode = PermutationValueSet.GetHashCode(); \
                        Config.Pack = Pack; \
                        Config.CustomHashCode = GetTemplateHashCode(Config.PermutationHashCode); \
                        Config.CustomBaseDependencyHashCode = Pack->GetBaseDependencyHashCodeForTemplates(); \
                        ABYTEK_FEEDBACK_STATUS_CHECK(Build(Config)); \
                        if (!Config.ShouldCompile) continue; \
                        OutConfigs.push_back(Config); \
                    } \
                     \
                    ABYTEK_LOG_INFO() << "Built configs for global render pipeline successfully: " << Abytek::TypeFullName<Name>(); \
                    return Abytek::F_FeedbackStatus::MakeSucceeded(); \
                } \
            ) \
            ABYTEK_ENABLE_IF_NOT_DEVELOPMENT_BUILD( \
                static Abytek::F_FeedbackStatus BuildConfigs( \
                    const Abytek::TS<Abytek::F_RenderPack>& Pack, \
                    const Abytek::TW_Valid<Abytek::F_SerializableEnvironment>& SerializableEnvironment, \
                    const Abytek::TW_Valid<Abytek::A_RenderPackTemplateMap>& TemplateMap, \
                    Abytek::TF_Vector<F_Config>& OutConfigs \
                ) \
                { \
                    return Abytek::F_FeedbackStatus::MakeSucceeded(); \
                } \
            ) \
            ABYTEK_ENABLE_IF_DEVELOPMENT_BUILD( \
                static Abytek::F_FeedbackStatus BuildCommandsAndCompilationSet( \
                    const Abytek::TS<Abytek::F_RenderPack>& Pack, \
                    const Abytek::TW_Valid<Abytek::F_SerializableEnvironment>& SerializableEnvironment, \
                    const Abytek::TW_Valid<Abytek::A_RenderPackTemplateMap>& TemplateMap, \
                    Abytek::TF_Vector<Abytek::TF_Function<Abytek::F_FeedbackStatus(Abytek::TF_Vector<Abytek::TS<Abytek::A_RHITemplate>>& OutTemplates)>>& OutCommands, \
                    Abytek::TF_Set<Abytek::F_RHITemplateHashCode>& OutTemplateHashCodesToCompile \
                ) \
                { \
                    auto Registry = Abytek::F_RenderRegistry::GetSerializableEnvironmentMetadataElement_Registry(SerializableEnvironment); \
                    auto Compiler = Registry->GetCompiler(); \
                    auto TemplateDatabase = Registry->GetTemplateDatabase(); \
                     \
                    Abytek::TF_Vector<F_Config> Configs; \
                    ABYTEK_FEEDBACK_STATUS_CHECK( \
                        BuildConfigs(Pack, SerializableEnvironment, TemplateMap, Configs) \
                    ); \
                    for (const auto& Config : Configs) \
                    { \
                        B8 ShouldCompile = Abytek::Internal::GlobalRenderPipeline::EnableInternalDebugger; \
                        if (TemplateMap->HasTemplate(*Config.CustomHashCode)) \
                        { \
                            Abytek::TW<Abytek::A_RHIPipelineStateTemplate> Template; \
                            if (TemplateMap->GetTemplate(*Config.CustomHashCode).TryDynamicCast<Abytek::A_RHIPipelineStateTemplate>(Template)) \
                            { \
                                const auto& LastSlangShaderFileVersions = Template->GetSlangShaderFileVersions(); \
                                if (Template->GetConfig() != static_cast<const Abytek::F_RHIPipelineStateTemplateConfig&>(Config)) \
                                { \
                                    ShouldCompile = true; \
                                } \
                                if (Template->GetCompileConfig() != static_cast<const Abytek::F_RHIPipelineStateTemplateCompileConfig&>(Config)) \
                                { \
                                    ShouldCompile = true; \
                                } \
                                Abytek::TF_Set<Abytek::F_Text> SlangShaderFilePaths; \
                                for (const auto& [_, SlangShaderFileVersion] : Template->GetSlangShaderFileVersions()) \
                                { \
                                    if (SlangShaderFilePaths.find(SlangShaderFileVersion.Path) != SlangShaderFilePaths.end()) continue; \
                                    SlangShaderFilePaths.insert(SlangShaderFileVersion.Path); \
                                } \
                                Config.ForEachShaderSource( \
                                    [&](const Abytek::F_RHIShaderSource& ShaderSource) \
                                    { \
                                        if (ShaderSource.Type == Abytek::E_RHIShaderSourceType::SLANG) \
                                        { \
                                            ShaderSource.Slang.ForEachModuleFile( \
                                                [&](const Abytek::F_Name& ModuleName, const Abytek::TF_Optional<Abytek::F_Text>& SlangShaderFilePath) \
                                                { \
                                                    ABYTEK_ENGINE_RENDER_CORE_ASSERT(SlangShaderFilePath) << "Not found slang shader file for module: " << ModuleName << ", in global render pipeline: " << Abytek::TypeFullName<Name>(); \
                                                    if (SlangShaderFilePaths.find(*SlangShaderFilePath) != SlangShaderFilePaths.end()) return; \
                                                    SlangShaderFilePaths.insert(*SlangShaderFilePath); \
                                                } \
                                            ); \
                                            return; \
                                        } \
                                        ShouldCompile = true; \
                                    } \
                                ); \
                                for (const auto& SlangShaderFilePath : SlangShaderFilePaths) \
                                { \
                                    auto SlangShaderFileVersion = Abytek::F_RHISlangShaderFileVersion::Make(SlangShaderFilePath); \
                                    if (!SlangShaderFileVersion.LoadCurrent()) \
                                    { \
                                        ShouldCompile = true; \
                                        continue; \
                                    } \
                                    auto It = LastSlangShaderFileVersions.find(SlangShaderFilePath); \
                                    if (It == LastSlangShaderFileVersions.end()) \
                                    { \
                                        ShouldCompile = true; \
                                        continue; \
                                    } \
                                    if (It->second.Hash != SlangShaderFileVersion.Hash) \
                                    { \
                                        ShouldCompile = true; \
                                        continue; \
                                    } \
                                } \
                                 \
                                for (const auto& BindGroup : Config.BindGroups) \
                                { \
                                    if (OutTemplateHashCodesToCompile.find(BindGroup.TemplateHashCode) != OutTemplateHashCodesToCompile.end()) \
                                    { \
                                        ShouldCompile = true; \
                                        break; \
                                    } \
                                } \
                            } \
                        } \
                        else \
                        { \
                            ShouldCompile = true; \
                        } \
                        if (ShouldCompile) \
                        { \
                            OutTemplateHashCodesToCompile.insert(*Config.CustomHashCode); \
                        } \
                        else \
                        { \
                            ABYTEK_LOG_INFO() << "Re-use precompiled global render pipeline: " << Abytek::TypeFullName<Name>() << ", permutation hash code: " << Config.PermutationHashCode << ", template hash code: " << *Config.CustomHashCode; \
                            continue; \
                        } \
                        OutCommands.push_back( \
                            [=](Abytek::TF_Vector<Abytek::TS<Abytek::A_RHITemplate>>& OutTemplates) -> Abytek::F_FeedbackStatus \
                            { \
                                ABYTEK_LOG_INFO() << "Compiling global render pipeline: " << Abytek::TypeFullName<Name>() << ", permutation hash code: " << Config.PermutationHashCode << ", template hash code: " << *Config.CustomHashCode; \
                                Abytek::TS<Abytek::A_RHIPipelineStateTemplate> PipelineStateTemplate; \
                                ABYTEK_FEEDBACK_STATUS_CHECK( \
                                    Compiler->CompilePipelineStateTemplate( \
                                        Config, \
                                        TemplateDatabase.Weak(), \
                                        PipelineStateTemplate \
                                    ) \
                                ); \
                                OutTemplates.push_back(PipelineStateTemplate); \
                                ABYTEK_LOG_INFO() << "Compiled global render pipeline: " << Abytek::TypeFullName<Name>() << ", permutation hash code: " << Config.PermutationHashCode << ", template hash code: " << *Config.CustomHashCode; \
                                return Abytek::F_FeedbackStatus::MakeSucceeded(); \
                            } \
                        ); \
                    } \
                    return Abytek::F_FeedbackStatus::MakeSucceeded(); \
                } \
            ) \
            ABYTEK_ENABLE_IF_NOT_DEVELOPMENT_BUILD( \
                static Abytek::F_FeedbackStatus BuildCommandsAndCompilationSet( \
                    const Abytek::TS<Abytek::F_RenderPack>& Pack, \
                    const Abytek::TW_Valid<Abytek::F_SerializableEnvironment>& SerializableEnvironment, \
                    const Abytek::TW_Valid<Abytek::A_RenderPackTemplateMap>& TemplateMap, \
                    Abytek::TF_Vector<Abytek::TF_Function<Abytek::F_FeedbackStatus(Abytek::TF_Vector<Abytek::TS<Abytek::A_RHITemplate>>& OutTemplates)>>& OutCommands, \
                    Abytek::TF_Set<Abytek::F_RHITemplateHashCode>& OutTemplateHashCodesToCompile \
                ) \
                { \
                    return Abytek::F_FeedbackStatus::MakeSucceeded(); \
                } \
            )
    
#define ABYTEK_MODULE_SHADER_SEARCH_PATHS() \
            Abytek::F_RenderCoreModule::GetInstance()->GetShaderSearchPaths(ABYTEK_MODULE_NAME)

namespace Abytek::Internal::GlobalRenderPipeline
{
    inline F_RHIShaderSourceSlang MakeShaderSourceSlang(const F_Name& EntryPointName, const F_Name& ModuleName, const TF_Vector<F_Text>& ShaderSearchPaths)
    {
        TF_Set<F_Text> ShaderSearchPathset;
        for (const auto& SearchPath_ : ShaderSearchPaths)
        {
            ShaderSearchPathset.insert(H_Path::Normalize(SearchPath_));
        }
        return F_RHIShaderSourceSlang::Make(
            EntryPointName,
            { ModuleName },
            ShaderSearchPathset,
            {}
        );
    }
}

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
#define ABYTEK_GLOBAL_SHADER(EntryPointName, ModuleName, ShaderFrequency, ...) \
            Abytek::TF_RHIPipelineStateTemplateShader<ShaderFrequency>::Make( \
                Abytek::F_RHIShaderSource::Make( \
                    Abytek::Internal::GlobalRenderPipeline::MakeShaderSourceSlang( \
                        ABYTEK_NAME(EntryPointName), \
                        ABYTEK_NAME(ModuleName), \
                        ABYTEK_MODULE_SHADER_SEARCH_PATHS() \
                    ) \
                ) \
            );
#else
#define ABYTEK_GLOBAL_SHADER(EntryPointName, ModuleName, ShaderFrequency, ...) \
            Abytek::TF_RHIPipelineStateTemplateShader<ShaderFrequency> {}
#endif