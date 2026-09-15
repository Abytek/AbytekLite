#include "Abytek/RenderRegistryRuntime.hpp"
#include "Abytek/RenderRegistry.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RenderRegistryRuntime)
    {
    }
    
    F_RenderRegistryRuntime::F_RenderRegistryRuntime(const F_RenderRegistryRuntimeBuildParams& BuildParams) :
        _Registry(BuildParams.Registry),
        _Context(BuildParams.Context),
        _RHIConfig(BuildParams.Registry->GetRHIConfig()),
        _TemplateRuntimeDatabase(BuildParams.Context->GetTemplateRuntimeDatabase().Weak())
    {
        for (const auto& DependencyRegistry : _Registry->GetDependencies())
        {
            _Dependencies.push_back(
                DependencyRegistry->GetOrActiveRuntime(_Context)
            );
        }
        _Registry->_TrackRuntime(ABYTEK_WTHIS());
    }
    F_RenderRegistryRuntime::~F_RenderRegistryRuntime()
    {
    }

    void F_RenderRegistryRuntime::FinalizeActivation()
    {
    }

    void F_RenderRegistryRuntime::FinalizeActivationAndUnlock()
    {
        FinalizeActivation();
        _IsActivated.test_and_set(boost::memory_order_release);
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
        if (auto Template = _Registry->QueryTemplate(HashCode))
        {
            if (auto TemplateRuntime = _TemplateRuntimeDatabase->GetOrActivateRuntime(Template))
            {
                return TemplateRuntime;
            }
        }  
        return {};
    }
}
