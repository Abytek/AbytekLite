#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIViewport.hpp"


namespace Abytek
{
    class A_RHIViewport;
    class A_RHIResourceProxy;
    class A_RHIResourceViewProxy;

    class ABYTEK_ENGINE_RHI_API A_RHIViewportProxy :
        public A_RHIContextChildProxy
    {
    private:
        TW<A_Window> _Window;
        F_Vector2_U32 _Size = F_Vector2_U32::Zero();
        
        E_RHIFormat _Format = E_RHIFormat::R8G8B8A8_UNORM;
        F_RHISampleDesc _SampleDesc;
        U32 _RefreshRate = 0;

        U32 _NumFrames = 2;
        
        TF_Vector<TS<A_RHIResourceProxy>> _FrameProxies;
        TF_Vector<TS<A_RHIResourceViewProxy>> _RenderTargetViewProxies;
        
        U32 _CurrentBackFrameIndex = 0;
        
    public:
        ABYTEK_FORCE_INLINE TW_Valid<A_RHIViewport> GetViewport() const noexcept
        {
            return GetContextChild().FastCast<A_RHIViewport>();
        }
        
        ABYTEK_FORCE_INLINE auto GetWindow() const noexcept
        {
            return _Window;
        }
        ABYTEK_FORCE_INLINE auto& InjectWindow() noexcept
        {
            return _Window;
        }
        ABYTEK_FORCE_INLINE auto GetSize() const noexcept
        {
            return _Size;
        }
        ABYTEK_FORCE_INLINE auto& InjectSize() noexcept
        {
            return _Size;
        }
        
        ABYTEK_FORCE_INLINE auto GetFormat() const noexcept
        {
            return _Format;
        }
        ABYTEK_FORCE_INLINE auto& InjectFormat() noexcept
        {
            return _Format;
        }
        ABYTEK_FORCE_INLINE auto GetSampleDesc() const noexcept
        {
            return _SampleDesc;
        }
        ABYTEK_FORCE_INLINE auto& InjectSampleDesc() noexcept
        {
            return _SampleDesc;
        }
        ABYTEK_FORCE_INLINE auto GetRefreshRate() const noexcept
        {
            return _RefreshRate;
        }
        ABYTEK_FORCE_INLINE auto& InjectRefreshRate() noexcept
        {
            return _RefreshRate;
        }
        
        ABYTEK_FORCE_INLINE auto GetNumFrames() const noexcept
        {
            return _NumFrames;
        }
        ABYTEK_FORCE_INLINE auto& InjectNumFrames() noexcept
        {
            return _NumFrames;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetFrameProxies() const noexcept
        {
            return _FrameProxies;
        }
        ABYTEK_FORCE_INLINE auto& InjectFrameProxies() noexcept
        {
            return _FrameProxies;
        }
        ABYTEK_FORCE_INLINE const auto& GetRenderTargetViewProxies() const noexcept
        {
            return _RenderTargetViewProxies;
        }
        ABYTEK_FORCE_INLINE auto& InjectRenderTargetViewProxies() noexcept
        {
            return _RenderTargetViewProxies;
        }

        ABYTEK_FORCE_INLINE auto GetCurrentBackFrameIndex() const noexcept
        {
            return _CurrentBackFrameIndex;
        }
        ABYTEK_FORCE_INLINE auto& InjectCurrentBackFrameIndex() noexcept
        {
            return _CurrentBackFrameIndex;
        }
        
    public:
        ABYTEK_FORCE_INLINE auto GetCurrentBackFrameProxy() const noexcept
        {
            return GetFrameProxies()[GetCurrentBackFrameIndex()];
        }
        ABYTEK_FORCE_INLINE auto GetCurrentBackRenderTargetViewProxy() const noexcept
        {
            return GetRenderTargetViewProxies()[GetCurrentBackFrameIndex()];
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIViewportProxy);
        void LateBuildViewport();
        virtual void Release() override;
    };
}
