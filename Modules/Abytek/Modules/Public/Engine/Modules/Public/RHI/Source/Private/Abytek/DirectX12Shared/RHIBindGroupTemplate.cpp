#include "Abytek/DirectX12Shared/RHIBindGroupTemplate.hpp"


namespace Abytek
{
    F_DirectX12SharedRHIBindGroupTemplate::F_DirectX12SharedRHIBindGroupTemplate(
        const F_RHIBindGroupTemplateBuildParams& BuildParams,
        F_DirectX12SharedRHIBindGroupTemplateCompiledData&& CompiledData
    ) :
        A_RHIBindGroupTemplate(BuildParams),
        _CompiledData(ABYTEK_MOVE(CompiledData))
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
}
