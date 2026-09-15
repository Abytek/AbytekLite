#pragma once

#include "Abytek/RHIClearDSVPass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIClearDSVPass : public A_RHIClearDSVPass, public A_DirectX12RHIPassExtension
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIClearDSVPass);
        virtual void Build(const F_RHIClearDSVPassBuildParams& BuildParams);
        virtual void Release() override;
        
    protected:
        virtual void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    };
}
#endif