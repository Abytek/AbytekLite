#pragma once

#include "Abytek/RHITemplateRuntime.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    struct F_DirectX12RHIRootSignatureTemplateRuntimeBuildParams : F_RHITemplateRuntimeBuildParams
    {
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIRootSignatureTemplateRuntime : public A_RHITemplateRuntime
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHIRootSignatureTemplateRuntime);
        virtual void Build(const F_DirectX12RHIRootSignatureTemplateRuntimeBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual void FinalizeActivation() override;
        
    protected:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;
        
    public:
        void InitD3D12RootSignature();
    };
}
#endif