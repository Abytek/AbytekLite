#pragma once

#include "Abytek/RHIResourceSyncPass.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIResourceSyncPass : public A_RHIResourceSyncPass, public A_DirectX12RHIPassExtension
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIResourceSyncPass);
        virtual void Build(const F_RHIResourceSyncPassBuildParams& BuildParams);
        void Release() override;
        
    protected:
        void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
    
    public:
        E_DirectX12RHIPassBatchType GetPassBatchType() override;
    };
}
#endif