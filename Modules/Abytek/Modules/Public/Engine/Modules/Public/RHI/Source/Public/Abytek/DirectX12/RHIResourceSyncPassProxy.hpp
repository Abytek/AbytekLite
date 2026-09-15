#pragma once

#include "Abytek/RHIResourceSyncPassProxy.hpp"
#include "Abytek/DirectX12/RHIPassProxyExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIResourceSyncPassProxy : public A_RHIResourceSyncPassProxy, public A_DirectX12RHIPassProxyExtension
    {
    private:
        
    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIResourceSyncPassProxy);
        virtual void Build(const TW_Valid<A_RHIResourceSyncPass>& Pass) override;
        virtual void Release() override;
    };
}
#endif