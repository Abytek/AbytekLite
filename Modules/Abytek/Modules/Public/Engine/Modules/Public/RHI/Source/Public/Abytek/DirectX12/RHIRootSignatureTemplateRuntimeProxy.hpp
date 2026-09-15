#pragma once

#include "Abytek/RHITemplateRuntime.hpp"
#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntime.hpp"
#include "Abytek/DirectX12/RHISubmissionContext.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIRootSignatureTemplateRuntimeProxy :
        public A_RHITemplateRuntimeProxy
    {
    private:
        Microsoft::WRL::ComPtr<ID3D12RootSignature> _D3D12RootSignature = nullptr;
        
    public:
        ABYTEK_FORCE_INLINE auto GetRootSignatureTemplateRuntime() const noexcept
        {
            return GetContextChild().FastCast<F_DirectX12RHIRootSignatureTemplateRuntime>();
        }
        
        ABYTEK_FORCE_INLINE const auto& GetD3D12RootSignature() const noexcept
        {
            return _D3D12RootSignature;
        }
        ABYTEK_FORCE_INLINE auto& InjectD3D12RootSignature() noexcept
        {
            return _D3D12RootSignature;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHIRootSignatureTemplateRuntimeProxy);
        void LateBuildRootSignatureTemplateRuntime();
        virtual void Release() override;
        
    public:
        void InitD3D12RootSignature();
        
    public:
        void PopulateCommandList(
            F_DirectX12RHISubmissionContext& SubmissionContext,
            E_RHIPassClass PassClass
        );
    };
}
#endif