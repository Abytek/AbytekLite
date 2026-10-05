#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"


namespace Abytek
{
    class A_WorldContext;
    class F_RenderRegistry;
    class F_RenderRegistryRuntime;
    class F_GlobalRenderPack;
    struct F_RenderCoreRHIConfig;
    
    struct ABYTEK_ENGINE_RENDER_CORE_API H_RenderCore
    {
        static TW<F_RenderRegistry> GetDefaultRegistry(
            const TW_Valid<A_WorldContext>& WorldContext
        );
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        static TS<A_RHICompiler> GetCompiler(
            const TW_Valid<A_WorldContext>& WorldContext,
            const TW<F_RenderRegistry>& Registry = {}
        );
#endif
        static const F_RenderCoreRHIConfig& GetRHIConfig(
            const TW_Valid<A_WorldContext>& WorldContext,
            const TW<F_RenderRegistry>& Registry = {}
        );
        static TS<A_RHITemplateDatabase> GetTemplateDatabase(
            const TW_Valid<A_WorldContext>& WorldContext,
            const TW<F_RenderRegistry>& Registry = {}
        );
        static TS<A_RHITemplateSerializer> GetTemplateSerializer(
            const TW_Valid<A_WorldContext>& WorldContext,
            const TW<F_RenderRegistry>& Registry = {}
        );
        static TS<A_RHITemplate> QueryTemplate(
            const TW_Valid<A_WorldContext>& WorldContext,
            F_RHITemplateHashCode HashCode,
            const TW<F_RenderRegistry>& Registry = {}
        );
        static TS<A_RHITemplate> GetTemplate(
            const TW_Valid<A_WorldContext>& WorldContext,
            F_RHITemplateHashCode HashCode,
            const TW<F_RenderRegistry>& Registry = {}
        );
        
        static TS<F_GlobalRenderPack> GetGlobalPack(
            const TW_Valid<A_WorldContext>& WorldContext    
        );
        
        template<typename __F_Namespace>
        static constexpr F_RHITemplateHashCode GenerateBaseDependencyHashCodeForTemplates(F_RHITemplateHashCode RawHashCode = 0)
        {
            F_RHITemplateHashCode Result = 0;
            Result = HashCombineU64(Result, TypeHashCode<__F_Namespace>);
            Result = HashCombineU64(Result, RawHashCode);
            return RHITemplateUserDefinedHashCode(Result);
        }
        template<typename __F_Namespace>
        static constexpr F_RHITemplateHashCode GenerateTemplateHashCode(F_RHITemplateHashCode TemplateHashCode, F_RHITemplateHashCode PackHashCode = 0)
        {
            F_RHITemplateHashCode Result = 0;
            Result = HashCombineU64(Result, TypeHashCode<__F_Namespace>);
            Result = HashCombineU64(Result, PackHashCode);
            Result = HashCombineU64(Result, TemplateHashCode);
            return RHITemplateUserDefinedHashCode(Result);
        }
    };
}