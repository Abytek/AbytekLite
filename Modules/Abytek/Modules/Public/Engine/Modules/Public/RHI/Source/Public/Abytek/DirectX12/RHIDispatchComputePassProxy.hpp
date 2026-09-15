#pragma once

#include "Abytek/RHIDispatchComputePassProxy.hpp"
#include "Abytek/DirectX12/RHIPassProxyExtension.hpp"
#include "Abytek/DirectX12/RHIHasGeneralPipelineProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIDispatchComputePassProxy : 
        public A_RHIDispatchComputePassProxy, 
        public A_DirectX12RHIPassProxyExtension,
        public A_DirectX12RHIHasGeneralPipelineProxy
    {
    private:
        
    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIDispatchComputePassProxy);
        virtual void Build(const TW_Valid<A_RHIDispatchComputePass>& Pass) override;
        virtual void Release() override;
        
    public:
        virtual void Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams) override;
    };
}
#endif