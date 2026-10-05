#include "Abytek/RHIPipelineStateTemplate.hpp"
#include "Abytek/RHIBindGroupTemplate.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntime.hpp"
#include "Abytek/RHITemplateDatabase.hpp"


namespace Abytek
{
    A_RHIPipelineStateTemplate::A_RHIPipelineStateTemplate(
        const TS<A_RHITemplateDatabase>& Database,
        F_RHITemplateHashCode HashCode,
        const F_RHIPipelineStateTemplateConfig& Config
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        , const F_RHIPipelineStateTemplateCompileConfig& CompileConfig
#endif
    ) :
        A_RHITemplate(
            Database,
            HashCode
        ),
        _Config(Config)
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        , _CompileConfig(CompileConfig)
#endif
    {
        for (const auto& BindGroup : Config.BindGroups)
        {
            F_RHITemplateHashCode BindGroupTemplateHashCode = BindGroup.TemplateHashCode;
            _BindGroupTemplates.push_back(
                Database->GetTemplate(BindGroupTemplateHashCode)
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

    void A_RHIPipelineStateTemplate::PostCreateExportedData(const TS<A_RHITemplateExportedData>& ExportedData) const
    {
        A_RHITemplate::PostCreateExportedData(ExportedData);
        const auto& CastedExportedData = ExportedData.FastCast<A_RHIPipelineStateTemplateExportedData>();
        CastedExportedData->Config = _Config;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        CastedExportedData->CompileConfig = _CompileConfig;
#endif
    }
}
