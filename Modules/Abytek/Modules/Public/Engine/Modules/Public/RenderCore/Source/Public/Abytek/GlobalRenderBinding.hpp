#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/RenderRegistryRuntime.hpp"
#include "Abytek/Permutation.hpp"
#include "Abytek/RenderPack.hpp"
#include "Abytek/ResourceUtilities.hpp"
#include "Abytek/RHIBindGroupTemplate.hpp"
#include "Abytek/RenderCoreHelper.hpp"


namespace Abytek
{
    class F_GlobalRenderPack;
    
    namespace Internal::GlobalRenderBinding
    {
#ifdef ABYTEK_ENABLE_INTERNAL_GLOBAL_RENDER_BINDING_DEBUG
        static constexpr B8 EnableInternalDebugger = true;
#else
        static constexpr B8 EnableInternalDebugger = false;
#endif
    }

    template<typename __F_DynamicPermutationVector>
    struct TF_GlobalRenderBindingConfig : F_RHIBindGroupTemplateCompileParams
    {
        B8 ShouldCompile = true;
        TS<F_RenderPack> Pack;
        __F_DynamicPermutationVector PermutationVector;
        F_RHITemplateHashCode PermutationHashCode;
        
        TF_Vector<F_RHITemplateHashCode> DependenciesHashCodes;
        void AddDependencyHashCode(F_RHITemplateHashCode DependencyHashCode) noexcept
        {
            if (
                std::find(
                    DependenciesHashCodes.begin(),
                    DependenciesHashCodes.end(),
                    DependencyHashCode
                )    
                != DependenciesHashCodes.end()
            )
            {
                return;
            }
            DependenciesHashCodes.push_back(DependencyHashCode);
        }
    };

    struct A_GlobalRenderBinding
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
            return ABYTEK_NAME("Abytek::A_GlobalRenderBinding::BuildCommandsAndCompilationSet");
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
        ABYTEK_END_REFLECTOR(A_GlobalRenderBinding);
        
        F_RHITemplateHashCode TemplateHashCode = INVALID_RHI_TEMPLATE_HASH_CODE;
        F_RHITemplateHashCode PermutationHashCode = 0;
        TS<A_RHIBindGroupTemplateRuntime> TemplateRuntime;

        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return TemplateRuntime.IsValid() && (TemplateHashCode != INVALID_RHI_TEMPLATE_HASH_CODE);
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        TS_Valid<A_RHIBindGroup> CreateBindGroup() const
        {
            ABYTEK_ENGINE_RENDER_CORE_ASSERT(IsValid()) << "Invalid binding, cannot create bind group";
            F_RHIBindGroupBuildParams BuildParams;
            BuildParams.Context = TemplateRuntime->GetContext();
            BuildParams.TemplateRuntime = TemplateRuntime;
            return RACreateAndBuildShared<A_RHIBindGroup>(BuildParams);
        }
        void Reset()
        {
            TemplateHashCode = 0;
            PermutationHashCode = 0;
            TemplateRuntime.Reset();
        }
        friend B8 operator == (const A_GlobalRenderBinding& A, const A_GlobalRenderBinding& B) noexcept
        {
            return (
                (A.TemplateHashCode == B.TemplateHashCode)    
                && (A.PermutationHashCode == B.PermutationHashCode)    
                && (A.TemplateRuntime == B.TemplateRuntime)    
            );
        }
        friend B8 operator != (const A_GlobalRenderBinding& A, const A_GlobalRenderBinding& B) noexcept
        {
            return (
                (A.TemplateHashCode != B.TemplateHashCode)    
                || (A.PermutationHashCode != B.PermutationHashCode)    
                || (A.TemplateRuntime != B.TemplateRuntime)    
            );
        }
    };
    
    template<class __F_Binding>
    struct TF_GlobalRenderBinding : A_GlobalRenderBinding
    {
        using F_Binding = __F_Binding;
        using F_PermutationDomain = F_DefaultPermutationDomain;
    };
}

#define ABYTEK_DECLARE_GLOBAL_RENDER_BINDING(Name) \
            ABYTEK_BEGIN_REFLECTOR(Abytek::A_GlobalRenderBinding) \
            ABYTEK_END_REFLECTOR(Name) \
             \
            using F_DynamicPermutationVector = typename F_PermutationDomain::F_DynamicVector; \
            using F_DefaultStaticPermutationVector = typename F_PermutationDomain::F_DefaultStaticVector; \
            using F_Config = Abytek::TF_GlobalRenderBindingConfig<F_DynamicPermutationVector>; \
            static constexpr Abytek::F_RHITemplateHashCode DefaultPermutationHashCode = F_DefaultStaticPermutationVector::GetHashCode(); \
             \
            static constexpr Abytek::F_RHITemplateHashCode GetTemplateHashCode(Abytek::F_RHITemplateHashCode InPermutationHashCode = DefaultPermutationHashCode) \
            { \
                Abytek::F_RHITemplateHashCode UseDefinedHashCode = 0; \
                UseDefinedHashCode = Abytek::HashCombineU64(UseDefinedHashCode, Abytek::TypeHashCode<F_Binding>); \
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
                TemplateRuntime = NewTemplateRuntime.FastCast<Abytek::A_RHIBindGroupTemplateRuntime>(); \
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
                ABYTEK_ENGINE_RENDER_CORE_ASSERT(Result.TemplateRuntime) << "Failed to instantiate global render binding: " << Abytek::TypeFullName<Name>(); \
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
                    ABYTEK_LOG_INFO() << "Building configs for global render binding: " << Abytek::TypeFullName<Name>(); \
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
                    ABYTEK_LOG_INFO() << "Built configs for global render binding successfully: " << Abytek::TypeFullName<Name>(); \
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
                        B8 ShouldCompile = Abytek::Internal::GlobalRenderBinding::EnableInternalDebugger; \
                        if (TemplateMap->HasTemplate(*Config.CustomHashCode)) \
                        { \
                            auto Template = TemplateMap->GetTemplate(*Config.CustomHashCode).FastCast<Abytek::A_RHIBindGroupTemplate>(); \
                            if (Template->GetConfig() != static_cast<const Abytek::F_RHIBindGroupTemplateConfig&>(Config)) \
                            { \
                                ShouldCompile = true; \
                            } \
                            if (Template->GetCompileConfig() != static_cast<const Abytek::F_RHIBindGroupTemplateCompileConfig&>(Config)) \
                            { \
                                ShouldCompile = true; \
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
                            ABYTEK_LOG_INFO() << "Re-use precompiled global render binding: " << Abytek::TypeFullName<Name>() << ", permutation hash code: " << Config.PermutationHashCode << ", template hash code: " << *Config.CustomHashCode; \
                            continue; \
                        } \
                        OutCommands.push_back( \
                            [=](Abytek::TF_Vector<Abytek::TS<Abytek::A_RHITemplate>>& OutTemplates) -> Abytek::F_FeedbackStatus \
                            { \
                                ABYTEK_LOG_INFO() << "Compiling global render binding: " << Abytek::TypeFullName<Name>() << ", permutation hash code: " << Config.PermutationHashCode << ", template hash code: " << *Config.CustomHashCode; \
                                Abytek::TS<Abytek::A_RHIBindGroupTemplate> BindGroupTemplate; \
                                ABYTEK_FEEDBACK_STATUS_CHECK( \
                                    Compiler->CompileBindGroupTemplate( \
                                        Config, \
                                        TemplateDatabase.Weak(), \
                                        BindGroupTemplate \
                                    ) \
                                ); \
                                OutTemplates.push_back(BindGroupTemplate); \
                                ABYTEK_LOG_INFO() << "Compiled global render binding: " << Abytek::TypeFullName<Name>() << ", permutation hash code: " << Config.PermutationHashCode << ", template hash code: " << *Config.CustomHashCode; \
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

#define ABYTEK_DEFINE_GLOBAL_RENDER_BINDING(...) \
            ABYTEK_REFLECT(__VA_ARGS__)