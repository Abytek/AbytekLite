#pragma once

#include "Abytek/RHIClearRTVPass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIClearRTVPass : public A_RHIClearRTVPass, public A_DirectX12RHIPassExtension
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIClearRTVPass);
        virtual void Build(const F_RHIClearRTVPassBuildParams& BuildParams);
        virtual void Release() override;
        
    protected:
        virtual void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    };
}
#endif