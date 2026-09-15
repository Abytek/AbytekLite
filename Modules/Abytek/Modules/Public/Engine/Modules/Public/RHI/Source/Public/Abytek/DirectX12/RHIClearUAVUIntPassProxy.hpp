#pragma once

#include "Abytek/RHIClearUAVUIntPassProxy.hpp"
#include "Abytek/DirectX12/RHIPassProxyExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIClearUAVUIntPassProxy : public A_RHIClearUAVUIntPassProxy, public A_DirectX12RHIPassProxyExtension
    {
    private:
        
    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIClearUAVUIntPassProxy);
        virtual void Build(const TW_Valid<A_RHIClearUAVUIntPass>& Pass) override;
        virtual void Release() override;
        
    public:
        virtual void Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams) override;
    };
}
#endif