#include "Abytek/DirectX12Shared/RHIBindGroupTemplate.hpp"


namespace Abytek
{
    F_DirectX12SharedRHIBindGroupTemplate::F_DirectX12SharedRHIBindGroupTemplate(
        const TW_Valid<A_RHITemplateDatabase>& Database,
        F_RHITemplateHashCode HashCode,
        const F_RHIBindGroupTemplateConfig& Config
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        , const F_RHIBindGroupTemplateCompileConfig& CompileConfig,
#endif
        const F_DirectX12SharedRHIBindGroupTemplateCompiledData& CompiledData
    ) :
        A_RHIBindGroupTemplate(
            Database,
            HashCode,
            Config
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
            , CompileConfig
#endif
        ),
        _CompiledData(CompiledData)
    {
    }
    F_DirectX12SharedRHIBindGroupTemplate::~F_DirectX12SharedRHIBindGroupTemplate()
    {
    }

    U32 F_DirectX12SharedRHIBindGroupTemplate::GetEncodedDataSizeInBytes()
    {
        return _CompiledData.EncodedDataSizeInBytes;
    }

    U32 F_DirectX12SharedRHIBindGroupTemplate::GetEncodedDataAlignmentInBytes()
    {
        return _CompiledData.EncodedDataAlignmentInBytes;
    }

    TS<A_RHITemplateExportedData> F_DirectX12SharedRHIBindGroupTemplate::CreateExportedData() const
    {
        return TS<F_DirectX12SharedRHIBindGroupTemplateExportedData>()();
    }
    void F_DirectX12SharedRHIBindGroupTemplate::PostCreateExportedData(const TS<A_RHITemplateExportedData>& ExportedData) const
    {
        A_RHIBindGroupTemplate::PostCreateExportedData(ExportedData);
        const auto& CastedExportedData = ExportedData.FastCast<F_DirectX12SharedRHIBindGroupTemplateExportedData>();
        CastedExportedData->CompiledData = _CompiledData;
    }
}
