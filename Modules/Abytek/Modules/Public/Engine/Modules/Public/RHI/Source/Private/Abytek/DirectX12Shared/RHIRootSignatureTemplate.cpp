#include "Abytek/DirectX12Shared/RHIRootSignatureTemplate.hpp"
#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntime.hpp"
#include "Abytek/RHIBindGroupTemplate.hpp"


namespace Abytek
{
    F_DirectX12SharedRHIRootSignatureTemplate::F_DirectX12SharedRHIRootSignatureTemplate(
        const F_DirectX12SharedRHIRootSignatureTemplateBuildParams& BuildParams,
        const F_DirectX12SharedRHIRootSignatureTemplateCompiledData& CompiledData
    ) :
        A_RHITemplate(BuildParams),
        _Config(static_cast<const F_DirectX12SharedRHIRootSignatureTemplateConfig&>(BuildParams)),
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
}
