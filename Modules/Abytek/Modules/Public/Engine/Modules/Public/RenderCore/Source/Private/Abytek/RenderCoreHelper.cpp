#include "Abytek/RenderCoreHelper.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/RenderRegistryRuntime.hpp"
#include "Abytek/RenderCoreManager.hpp"
#include "Abytek/World/WorldSubsystemContainer.hpp"


namespace Abytek
{
    TW<F_RenderRegistry> H_RenderCore::GetDefaultRegistry(
        const TW_Valid<A_WorldContext>& WorldContext
    )
    {
        return H_WorldContext::GetUnit<F_RenderCoreManager>(WorldContext)->GetMainRegistry().Weak();
    }
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    TS<A_RHICompiler> H_RenderCore::GetCompiler(
            const TW_Valid<A_WorldContext>& WorldContext,
            const TW<F_RenderRegistry>& Registry
        )
    {
        auto ActualRegistry = Registry;
        if (!ActualRegistry)
        {
            ActualRegistry = GetDefaultRegistry(WorldContext);
        }
        return ActualRegistry->GetCompiler();
    }
#endif
    const F_RenderCoreRHIConfig& H_RenderCore::GetRHIConfig(
        const TW_Valid<A_WorldContext>& WorldContext,
        const TW<F_RenderRegistry>& Registry
    )
    {
        auto ActualRegistry = Registry;
        if (!ActualRegistry)
        {
            ActualRegistry = GetDefaultRegistry(WorldContext);
        }
        return ActualRegistry->GetRHIConfig();
    }
    TS<A_RHITemplateDatabase> H_RenderCore::GetTemplateDatabase(
        const TW_Valid<A_WorldContext>& WorldContext,
        const TW<F_RenderRegistry>& Registry
    )
    {
        auto ActualRegistry = Registry;
        if (!ActualRegistry)
        {
            ActualRegistry = GetDefaultRegistry(WorldContext);
        }
        return ActualRegistry->GetTemplateDatabase();
    }
    TS<A_RHITemplateSerializer> H_RenderCore::GetTemplateSerializer(
        const TW_Valid<A_WorldContext>& WorldContext,
        const TW<F_RenderRegistry>& Registry
    )
    {
        auto ActualRegistry = Registry;
        if (!ActualRegistry)
        {
            ActualRegistry = GetDefaultRegistry(WorldContext);
        }
        return ActualRegistry->GetTemplateSerializer();
    }
    TS<A_RHITemplate> H_RenderCore::QueryTemplate(
        const TW_Valid<A_WorldContext>& WorldContext,
        F_RHITemplateHashCode HashCode, 
        const TW<F_RenderRegistry>& Registry
    )
    {
        auto ActualRegistry = Registry;
        if (!ActualRegistry)
        {
            ActualRegistry = GetDefaultRegistry(WorldContext);
        }
        return ActualRegistry->QueryTemplate(HashCode);
    }
    TS<A_RHITemplate> H_RenderCore::GetTemplate(
        const TW_Valid<A_WorldContext>& WorldContext,
        F_RHITemplateHashCode HashCode, 
        const TW<F_RenderRegistry>& Registry
    )
    {
        auto ActualRegistry = Registry;
        if (!ActualRegistry)
        {
            ActualRegistry = GetDefaultRegistry(WorldContext);
        }
        return ActualRegistry->QueryTemplate(HashCode);
    }

    TS<F_GlobalRenderPack> H_RenderCore::GetGlobalPack(
        const TW_Valid<A_WorldContext>& WorldContext
    )
    {
        return H_WorldContext::GetUnit<F_RenderCoreManager>(WorldContext)->GetGlobalRenderPack();
    }
}
