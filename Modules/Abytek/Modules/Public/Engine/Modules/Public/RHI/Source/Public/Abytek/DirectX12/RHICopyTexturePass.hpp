#pragma once

#include "Abytek/RHICopyTexturePass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHICopyTexturePass : public A_RHICopyTexturePass, public A_DirectX12RHIPassExtension
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHICopyTexturePass);
        virtual void Build(const F_RHICopyTexturePassBuildParams& BuildParams);
        virtual void Release() override;
        
    protected:
        virtual void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    };
}
#endif