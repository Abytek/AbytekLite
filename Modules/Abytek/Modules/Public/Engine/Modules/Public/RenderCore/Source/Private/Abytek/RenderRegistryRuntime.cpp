#include "Abytek/RenderRegistryRuntime.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RenderRegistryRuntime)
    {
    }
    
    F_RenderRegistryRuntime::F_RenderRegistryRuntime(const F_RenderRegistryRuntimeBuildParams& BuildParams) :
        _Context(BuildParams.Context),
        _RHIFeatureSupports(BuildParams.RHIFeatureSupports),
        _Dependencies(BuildParams.Dependencies),
        _TemplateRuntimeDatabase(BuildParams.Context->GetTemplateRuntimeDatabase().Weak())
    {
        _TemplateDatabase = A_RHITemplateDatabase::Create(
            F_RHISubsystem::GetInstance()->GetActiveAPI(),
            _RHIFeatureSupports
        );
    }
    F_RenderRegistryRuntime::~F_RenderRegistryRuntime()
    {
        _TemplateDatabase = {};
    }

    TS<A_RHITemplateRuntime> F_RenderRegistryRuntime::QueryTemplateRuntime(F_RHITemplateHashCode HashCode)
    {
        return _TemplateRuntimeDatabase->Find(HashCode);
    }
    TS<A_RHITemplateRuntime> F_RenderRegistryRuntime::GetOrActivate(F_RHITemplateHashCode HashCode)
    {
        if (auto QueriedTemplateRuntime = QueryTemplateRuntime(HashCode))
        {
            return QueriedTemplateRuntime;
        }
        for (const auto& Dependency : GetDependencies())
        {
            if (auto TemplateRuntime = Dependency->GetOrActivate(HashCode))
            {
                return TemplateRuntime;
            }
        }
        if (auto Template = _TemplateDatabase->FindTemplate(HashCode))
        {
            if (auto TemplateRuntime = _TemplateRuntimeDatabase->GetOrActivateRuntime(Template))
            {
                return TemplateRuntime;
            }
        }  
        return {};
    }
}
