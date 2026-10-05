#include "Abytek/DirectX12Shared/RHIRootSignatureTemplate.hpp"
#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntime.hpp"
#include "Abytek/RHIBindGroupTemplate.hpp"


namespace Abytek
{
    F_DirectX12SharedRHIRootSignatureTemplate::F_DirectX12SharedRHIRootSignatureTemplate(
        const TW_Valid<A_RHITemplateDatabase>& Database,
        F_RHITemplateHashCode HashCode,
        const F_DirectX12SharedRHIRootSignatureTemplateConfig& Config
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        , const F_DirectX12SharedRHIRootSignatureTemplateCompileConfig& CompileConfig,
#endif
        const F_DirectX12SharedRHIRootSignatureTemplateCompiledData& CompiledData
    ) :
        A_RHITemplate(
            Database,
            HashCode
        ),
        _Config(Config),
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        _CompileConfig(CompileConfig),
#endif
        _CompiledData(CompiledData)
    {
    }
    F_DirectX12SharedRHIRootSignatureTemplate::~F_DirectX12SharedRHIRootSignatureTemplate()
    {
    }

    TS_Valid<A_RHITemplateRuntime> F_DirectX12SharedRHIRootSignatureTemplate::CreateAndBuildRuntime(const TW_Valid<A_RHIContext>& Context)
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        F_DirectX12RHIRootSignatureTemplateRuntimeBuildParams BuildParams;
        BuildParams.Context = Context;
        BuildParams.Template = ABYTEK_STHIS();
        return RACreateAndBuildShared<F_DirectX12RHIRootSignatureTemplateRuntime>(BuildParams);
#else
        return {};
#endif
    }

    TS<A_RHITemplateExportedData> F_DirectX12SharedRHIRootSignatureTemplate::CreateExportedData() const
    {
        return TS<F_DirectX12SharedRHIRootSignatureTemplateExportedData>()();
    }
    void F_DirectX12SharedRHIRootSignatureTemplate::PostCreateExportedData(const TS<A_RHITemplateExportedData>& ExportedData) const
    {
        A_RHITemplate::PostCreateExportedData(ExportedData);
        const auto& CastedExportedData = ExportedData.FastCast<F_DirectX12SharedRHIRootSignatureTemplateExportedData>();
        CastedExportedData->Config = _Config;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        CastedExportedData->CompileConfig = _CompileConfig;
#endif
        CastedExportedData->CompiledData = _CompiledData;
    }
}
