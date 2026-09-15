#pragma once

#include "Abytek/RHIPipelineStateTemplateRuntimeProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHIRootSignatureTemplateRuntimeProxy;
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIPipelineStateTemplateRuntimeProxy : public A_RHIPipelineStateTemplateRuntimeProxy
    {
    private:
        TS<F_DirectX12RHIRootSignatureTemplateRuntimeProxy> _RootSignatureTemplateRuntimeProxy;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> _D3D12PipelineState = nullptr;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRootSignatureTemplateRuntimeProxy() const noexcept
        {
            return _RootSignatureTemplateRuntimeProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectRootSignatureTemplateRuntimeProxy() noexcept
        {
            return _RootSignatureTemplateRuntimeProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetD3D12PipelineState() const noexcept
        {
            return _D3D12PipelineState;
        }
        ABYTEK_FORCE_INLINE auto& InjectD3D12PipelineState() noexcept
        {
            return _D3D12PipelineState;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIPipelineStateTemplateRuntimeProxy);
        void LateBuildPipelineStateTemplateRuntime_DirectX12();
        virtual void Release() override;
        
    public:
        void InitD3D12PipelineState();
        
    private: 
        void _InitGraphics();
        void _InitCompute();
        void _InitWorkGraph();
    };
}
#endif