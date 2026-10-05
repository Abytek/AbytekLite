#include "Abytek/DirectX12Shared/RHIPipelineStateTemplate.hpp"
#include "Abytek/RHITemplateDatabase.hpp"


namespace Abytek
{
    F_DirectX12SharedRHIPipelineStateTemplate::F_DirectX12SharedRHIPipelineStateTemplate(
        const TS<A_RHITemplateDatabase>& Database,
        F_RHITemplateHashCode HashCode,
        const F_RHIPipelineStateTemplateConfig& Config,
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        const F_RHIPipelineStateTemplateCompileConfig& CompileConfig,
#endif
        const F_DirectX12SharedRHIPipelineStateTemplateCompiledData& CompiledData
    ) :
        A_RHIPipelineStateTemplate(
            Database,
            HashCode,
            Config
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
            , CompileConfig
#endif
        ),
        _CompiledData(CompiledData)
    {
        {
            F_RHITemplateHashCode RootSignatureTemplateHashCode = _CompiledData.RootSignatureTemplateHashCode;
            _RootSignatureTemplate = Database->GetTemplate(RootSignatureTemplateHashCode)
                .FastCast<F_DirectX12SharedRHIRootSignatureTemplate>();
            EnsureDependencyHashCode(RootSignatureTemplateHashCode);
        }
    }
    F_DirectX12SharedRHIPipelineStateTemplate::~F_DirectX12SharedRHIPipelineStateTemplate()
    {
    }

    TS<A_RHITemplateExportedData> F_DirectX12SharedRHIPipelineStateTemplate::CreateExportedData() const
    {
        return TS<F_DirectX12SharedRHIPipelineStateTemplateExportedData>()();
    }
    void F_DirectX12SharedRHIPipelineStateTemplate::PostCreateExportedData(const TS<A_RHITemplateExportedData>& ExportedData) const
    {
        A_RHIPipelineStateTemplate::PostCreateExportedData(ExportedData);
        const auto& CastedExportedData = ExportedData.FastCast<F_DirectX12SharedRHIPipelineStateTemplateExportedData>();
        CastedExportedData->CompiledData = _CompiledData;
    }
}
