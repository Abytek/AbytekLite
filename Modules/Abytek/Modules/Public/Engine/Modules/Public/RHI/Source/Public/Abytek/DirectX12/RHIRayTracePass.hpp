#pragma once

#include "Abytek/RHIRayTracePass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"
#include "Abytek/DirectX12/RHIHasGeneralPipeline.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIRayTracePass : public A_RHIRayTracePass, public A_DirectX12RHIPassExtension, public A_DirectX12RHIHasGeneralPipeline
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIRayTracePass);
        virtual void Build(const F_RHIRayTracePassBuildParams& BuildParams);
        virtual void Release() override;
        
    protected:
        virtual void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    };
}
#endif