#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIFormat.hpp"
#include "Abytek/RHISampleDesc.hpp"
#include "Abytek/RHIDrawPass.hpp"


namespace Abytek
{
    class A_RHIResource;
    class A_RHIResourceView;
    class A_Window;

    struct F_RHIViewportBuildParams : F_RHIContextChildBuildParams
    {
        TW<A_Window> Window;
        F_Vector2_U32 Size;
        
        E_RHIFormat Format = E_RHIFormat::R8G8B8A8_UNORM;
        F_RHISampleDesc SampleDesc;
        U32 RefreshRate = 0;

        U32 NumFrames = 2;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIViewport : public A_RHIContextChild
    {
    private:
        TW<A_Window> _Window;
        F_Vector2_U32 _Size = F_Vector2_U32::Zero();
        
        E_RHIFormat _Format = E_RHIFormat::R8G8B8A8_UNORM;
        F_RHISampleDesc _SampleDesc;
        U32 _RefreshRate = 0;

        U32 _NumFrames = 2;
        
        TF_Vector<TS<A_RHIResource>> _Frames;
        TF_Vector<TS<A_RHIResourceView>> _RenderTargetViews;
        
        U32 _CurrentBackFrameIndex = 0;

    public:
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
        
        ABYTEK_FORCE_INLINE const auto& GetFrames() const noexcept
        {
            return _Frames;
        }
        ABYTEK_FORCE_INLINE auto& InjectFrames() noexcept
        {
            return _Frames;
        }
        ABYTEK_FORCE_INLINE const auto& GetRenderTargetViews() const noexcept
        {
            return _RenderTargetViews;
        }
        ABYTEK_FORCE_INLINE auto& InjectRenderTargetViews() noexcept
        {
            return _RenderTargetViews;
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
        ABYTEK_FORCE_INLINE auto GetCurrentBackFrame() const noexcept
        {
            return GetFrames()[GetCurrentBackFrameIndex()];
        }
        ABYTEK_FORCE_INLINE auto GetCurrentBackRenderTargetView() const noexcept
        {
            return GetRenderTargetViews()[GetCurrentBackFrameIndex()];
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIViewport);
        virtual void Build(const F_RHIViewportBuildParams& BuildParams);
        virtual void Release();
        
    public:
        virtual void Resize(const F_Vector2_U32& NewSize);
        
    protected:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;
        
    public:
        F_RHIViewportScissorConfig GetDefaultViewportScissorConfig() const;
    };
}
