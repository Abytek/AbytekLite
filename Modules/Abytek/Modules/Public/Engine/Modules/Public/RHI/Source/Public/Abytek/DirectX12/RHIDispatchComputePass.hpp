#pragma once

#include "Abytek/RHIDispatchComputePass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"
#include "Abytek/DirectX12/RHIHasGeneralPipeline.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIDispatchComputePass : public A_RHIDispatchComputePass, public A_DirectX12RHIPassExtension, public A_DirectX12RHIHasGeneralPipeline
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIDispatchComputePass);
        virtual void Build(const F_RHIDispatchComputePassBuildParams& BuildParams);
        virtual void Release() override;
        
    protected:
        virtual void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    };
}
#endif