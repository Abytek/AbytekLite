#include "Abytek/DirectX12Shared/RHIPipelineStateTemplate.hpp"
#include "Abytek/RHITemplateDatabase.hpp"


namespace Abytek
{
    F_DirectX12SharedRHIPipelineStateTemplate::F_DirectX12SharedRHIPipelineStateTemplate(
        const F_RHIPipelineStateTemplateBuildParams& BuildParams,
        F_DirectX12SharedRHIPipelineStateTemplateCompiledData&& CompiledData
    ) :
        A_RHIPipelineStateTemplate(BuildParams),
        _CompiledData(ABYTEK_MOVE(CompiledData))
    {
        {
            F_RHITemplateHashCode RootSignatureTemplateHashCode = _CompiledData.RootSignatureTemplateHashCode;
            _RootSignatureTemplate = BuildParams.Database->GetTemplate(RootSignatureTemplateHashCode)
                .FastCast<F_DirectX12SharedRHIRootSignatureTemplate>();
            EnsureDependencyHashCode(RootSignatureTemplateHashCode);
        }
    }
    F_DirectX12SharedRHIPipelineStateTemplate::~F_DirectX12SharedRHIPipelineStateTemplate()
    {
    }
}
