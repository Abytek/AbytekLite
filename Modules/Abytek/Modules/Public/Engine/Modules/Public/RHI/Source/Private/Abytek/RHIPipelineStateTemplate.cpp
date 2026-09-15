#include "Abytek/RHIPipelineStateTemplate.hpp"
#include "Abytek/RHIBindGroupTemplate.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntime.hpp"
#include "Abytek/RHITemplateDatabase.hpp"


namespace Abytek
{
    A_RHIPipelineStateTemplate::A_RHIPipelineStateTemplate(const F_RHIPipelineStateTemplateBuildParams& BuildParams) :
        A_RHITemplate(BuildParams),
        _Config(static_cast<const F_RHIPipelineStateTemplateConfig&>(BuildParams))
    {
        for (const auto& BindGroup : BuildParams.BindGroups)
        {
            F_RHITemplateHashCode BindGroupTemplateHashCode = BindGroup.TemplateHashCode;
            _BindGroupTemplates.push_back(
                BuildParams.Database->GetTemplate(BindGroupTemplateHashCode)
                .FastCast<A_RHIBindGroupTemplate>()
            );
            EnsureDependencyHashCode(BindGroupTemplateHashCode);
        }
    }
    A_RHIPipelineStateTemplate::~A_RHIPipelineStateTemplate()
    {
    }

    TS_Valid<A_RHITemplateRuntime> A_RHIPipelineStateTemplate::CreateAndBuildRuntime(const TW_Valid<A_RHIContext>& Context)
    {
        F_RHIPipelineStateTemplateRuntimeBuildParams BuildParams;
        BuildParams.Context = Context;
        BuildParams.Template = ABYTEK_STHIS();
        return RACreateAndBuildShared<A_RHIPipelineStateTemplateRuntime>(BuildParams);
    }
}
