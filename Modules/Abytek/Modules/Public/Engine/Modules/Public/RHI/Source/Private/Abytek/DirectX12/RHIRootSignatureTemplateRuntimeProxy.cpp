#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12/RHIContextProxy.hpp"
#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntime.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/DirectX12Shared/RHIRootSignatureTemplate.hpp" 


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHIRootSignatureTemplateRuntimeProxy)
    void F_DirectX12RHIRootSignatureTemplateRuntimeProxy::LateBuildRootSignatureTemplateRuntime()
    {
        auto RootSignatureTemplateRuntime = GetRootSignatureTemplateRuntime();
    }
    void F_DirectX12RHIRootSignatureTemplateRuntimeProxy::Release()
    {
        _D3D12RootSignature = nullptr;
        A_RHITemplateRuntimeProxy::Release();
    }

    void F_DirectX12RHIRootSignatureTemplateRuntimeProxy::InitD3D12RootSignature()
    {
        auto CastedTemplate = GetTemplate().FastCast<F_DirectX12SharedRHIRootSignatureTemplate>();
        const auto& CompiledData = CastedTemplate->GetCompiledData();
    
        auto D3D12Device = GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        HRESULT HR = D3D12Device->CreateRootSignature(
            1,
            CompiledData.Binary.data(),
            CompiledData.Binary.size(),
            IID_PPV_ARGS(&_D3D12RootSignature)
        );
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot create d3d12 root signature";
    }

    void F_DirectX12RHIRootSignatureTemplateRuntimeProxy::PopulateCommandList(
        F_DirectX12RHISubmissionContext& SubmissionContext,
        E_RHIPassClass PassClass
    )
    {
        SubmissionContext.Update_D3D12RootSignature(PassClass, _D3D12RootSignature);
    }
}
#endif