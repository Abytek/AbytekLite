#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"
#include "Abytek/DirectX12/RHISubmissionContext.hpp"
#include "Abytek/RHICaptureEventState.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHICommandQueueProxy;
    
    struct F_DirectX12RHIPassProxyExtensionExecuteParams : F_DirectX12RHISubmissionContext
    {
        TW<A_RHIProcess> Process;
        U32 PassBatchIndex = ~U32(0);
        TW<F_DirectX12RHICommandQueueProxy> CommandQueueProxy;
    };
    
    class ABYTEK_ENGINE_RHI_API A_DirectX12RHIPassProxyExtension
    {
    private:
        TW<A_RHIPassProxy> _PassProxy;
        F_DirectX12RHIResourceBarrierProxySet _ResourceBarrierProxies_Before;
        F_DirectX12RHIResourceBarrierProxySet _ResourceBarrierProxies_After;
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        TF_SmallVector<F_RHICaptureEventState, 1> _CaptureEventStatesToBegin;
        TF_SmallVector<F_RHICaptureEventState, 1> _CaptureEventStatesToEnd;
#endif
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPassProxy() const noexcept
        {
            return _PassProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetResourceBarrierProxies_Before() const noexcept
        {
            return _ResourceBarrierProxies_Before;
        }
        ABYTEK_FORCE_INLINE const auto& GetResourceBarrierProxies_After() const noexcept
        {
            return _ResourceBarrierProxies_After;
        }
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        ABYTEK_FORCE_INLINE const auto& GetCaptureEventStatesToBegin() const noexcept
        {
            return _CaptureEventStatesToBegin;
        }
        ABYTEK_FORCE_INLINE const auto& GetCaptureEventStatesToEnd() const noexcept
        {
            return _CaptureEventStatesToEnd;
        }
#endif
        
    public:
        A_DirectX12RHIPassProxyExtension();
        virtual ~A_DirectX12RHIPassProxyExtension();

    public:
        void Build(const TW_Valid<A_RHIPass>& Pass);
        void Release();
        
    public:
        virtual void Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams);
    };
}
#endif