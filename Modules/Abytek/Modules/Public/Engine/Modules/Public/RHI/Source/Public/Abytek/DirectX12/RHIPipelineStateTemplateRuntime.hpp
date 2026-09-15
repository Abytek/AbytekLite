#pragma once

#include "Abytek/RHIPipelineStateTemplateRuntime.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHIRootSignatureTemplateRuntime;
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIPipelineStateTemplateRuntime : public A_RHIPipelineStateTemplateRuntime
    {
    private:
        TS<F_DirectX12RHIRootSignatureTemplateRuntime> _RootSignatureTemplateRuntime;

    public:
        ABYTEK_FORCE_INLINE const auto& GetRootSignatureTemplateRuntime() const noexcept
        {
            return _RootSignatureTemplateRuntime;
        }
        ABYTEK_FORCE_INLINE auto& InjectRootSignatureTemplateRuntime() noexcept
        {
            return _RootSignatureTemplateRuntime;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIPipelineStateTemplateRuntime);
        virtual void Build(const F_RHIPipelineStateTemplateRuntimeBuildParams& BuildParams) override;
        virtual void Release() override;

    protected:
        virtual void FinalizeActivation() override;
        
    public:
        void InitD3D12PipelineState();
    };
}
#endif