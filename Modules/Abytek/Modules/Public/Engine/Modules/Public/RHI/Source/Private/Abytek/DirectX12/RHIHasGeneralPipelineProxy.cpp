#include "Abytek/DirectX12/RHIHasGeneralPipelineProxy.hpp"
#include "Abytek/RHIBindGroupTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12/RHIDescriptorManagerProxy.hpp"
#include "Abytek/DirectX12/RHIPipelineStateProxy.hpp"
#include "Abytek/DirectX12/RHIPipelineStateTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12/RHIBindGroupProxy.hpp"
#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12Shared/RHIBindGroupTemplate.hpp"
#include "Abytek/DirectX12Shared/RHIPipelineStateTemplate.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void A_DirectX12RHIHasGeneralPipelineProxy::Build(const TW_Valid<A_RHIPass>& Pass)
    {
        _PassProxy = ABYTEK_WTHIS().DynamicCast<A_RHIPassProxy>();
        _PassProxyExtension = ABYTEK_WTHIS().DynamicCast<A_DirectX12RHIPassProxyExtension>();
        _Base = ABYTEK_WTHIS().DynamicCast<A_RHIHasGeneralPipelineProxy>();
        
        _PassClass = _PassProxy->GetPassClass();
    }
    void A_DirectX12RHIHasGeneralPipelineProxy::Release()
    {
        _PassClass = E_RHIPassClass::NONE;
        
        _Base = {};
        _PassProxyExtension = {};
        _PassProxy = {};
    }

    void A_DirectX12RHIHasGeneralPipelineProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        auto CastedTemplateRuntimeProxy = _Base->GetPipelineStateProxy().FastCast<F_DirectX12RHIPipelineStateTemplateRuntimeProxy>();
        auto CastedTemplate = CastedTemplateRuntimeProxy->GetTemplate().FastCast<F_DirectX12SharedRHIPipelineStateTemplate>();
        
        auto D3D12PipelineState = CastedTemplateRuntimeProxy->GetD3D12PipelineState();
        
        const auto& PipelineStateTemplateCompiledData = CastedTemplate->GetCompiledData();
        const auto& BindGroupMaps = PipelineStateTemplateCompiledData.BindGroupMaps;
        
        const auto& BindGroupProxies = _Base->GetBindGroupProxies();
        
        H_DirectX12RHIDescriptorManagerProxy::PopulateCommandList(
            ExecuteParams,
            _PassProxy->GetContextProxy().WithValidation()
        );
        CastedTemplateRuntimeProxy->GetRootSignatureTemplateRuntimeProxy()->PopulateCommandList(
            ExecuteParams,
            _PassClass
        );
        H_DirectX12RHIBindGroupProxy::PopulateCommandList(
            ExecuteParams,
            BindGroupProxies,
            BindGroupMaps,
            _PassClass
        );
        ExecuteParams.Update_D3D12PipelineState(D3D12PipelineState);
        if (CastedTemplate->GetType() == E_RHIPipelineStateType::GRAPHICS)
        {
            ExecuteParams.Update_PrimitiveTopology(CastedTemplate->GetPrimitiveTopology());
        }
    }
}
#endif