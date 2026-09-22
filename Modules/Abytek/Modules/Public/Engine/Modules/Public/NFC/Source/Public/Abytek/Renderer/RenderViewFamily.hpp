#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/RenderView.hpp"


namespace Abytek
{
    class F_CanvasRenderProxy;
    class F_CameraRenderProxy;
    class A_RenderScene;
    class A_RenderView;

    struct F_RenderViewFamilyBuildParams
    {
        TW<F_CanvasRenderProxy> CanvasRenderProxy;
        TW<F_CameraRenderProxy> CameraRenderProxy;
        TW<A_RenderScene> Scene;
    };
    class ABYTEK_ENGINE_NFC_API A_RenderViewFamily : public A_RenderObject
    {
    private:
        TW<F_CanvasRenderProxy> _CanvasRenderProxy;
        TW<F_CameraRenderProxy> _CameraRenderProxy;
        TW<A_RenderScene> _Scene;
        TF_Vector<TS<A_RenderView>> _Views;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetCanvasRenderProxy() const noexcept
        {
            return _CanvasRenderProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetCameraRenderProxy() const noexcept
        {
            return _CameraRenderProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        ABYTEK_FORCE_INLINE const auto& GetViews() const noexcept
        {
            return _Views;
        }
        
    public:
        ABYTEK_RENDER_OBJECT(A_RenderViewFamily, A_RenderObject);
        
    public:
        virtual void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RenderViewFamilyBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void AddView(const TS<A_RenderView>& View);
        
    public:
        B8 HasView(E_RenderViewFlag Flags) const noexcept
        {
            for (const auto& View : _Views)
            {
                if (View->HasFlags(Flags))
                {
                    return true;
                }
            }
            return false;
        }
        TS<A_RenderView> FindView(E_RenderViewFlag Flags) const noexcept
        {
            for (const auto& View : _Views)
            {
                if (View->HasFlags(Flags))
                {
                    return View;
                }
            }
            return {};
        }
        TS_Valid<A_RenderView> GetView(E_RenderViewFlag Flags) const noexcept
        {
            for (const auto& View : _Views)
            {
                if (View->HasFlags(Flags))
                {
                    return View;
                }
            }
            ABYTEK_LOG_FATAL() << "Not found view with flags: " << static_cast<U32>(Flags);
            return {};
        }
        
    protected:
        virtual void OnBeginFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        virtual void OnEndFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        void BeginFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}
