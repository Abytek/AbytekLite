#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIHasGeneralPipelineProxy.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIPassProxyExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API A_DirectX12RHIHasGeneralPipelineProxy
    {
    private:
        TW<A_RHIPassProxy> _PassProxy;
        TW<A_DirectX12RHIPassProxyExtension> _PassProxyExtension;
        TW<A_RHIHasGeneralPipelineProxy> _Base;
        
        E_RHIPassClass _PassClass = E_RHIPassClass::NONE;

    public:

    public:
        A_DirectX12RHIHasGeneralPipelineProxy() = default;
        virtual ~A_DirectX12RHIHasGeneralPipelineProxy() = default;
        void Build(const TW_Valid<A_RHIPass>& Pass);
        void Release();
        
    public:
        void Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams);
    };
}
#endif