#pragma once

#include "Abytek/RHIViewportProxy.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIViewportProxy : public A_RHIViewportProxy
    {
    private:
        IDXGISwapChain3* _DXGISwapChain;
        mutable F_YieldSpinLock _DXGISwapChainLock;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetDXGISwapChain() const noexcept
        {
            return _DXGISwapChain;
        }
        ABYTEK_FORCE_INLINE auto& InjectDXGISwapChain() noexcept
        {
            return _DXGISwapChain;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIViewportProxy);
        void LateBuildViewport_DirectX12();
        virtual void Release() override;
        
    public:
        void InitDXGISwapchain();
        void ResizeDXGISwapchain(F_Vector2_U32 NewSize);
        
    private:
        void _LateSetupDXGISwapchain();
        void _AssignViewportFrames();
        
    public:
        void LockDXGISwapChain();
        void UnlockDXGISwapChain();
    };
}
#endif