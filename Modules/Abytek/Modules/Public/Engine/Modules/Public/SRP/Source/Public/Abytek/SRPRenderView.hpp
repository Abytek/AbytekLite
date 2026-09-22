#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/SRPCommon.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPRenderView final : public A_RenderView
    {
    private:
        TS<A_RHIResource> _OpaqueVisibilityBuffer;
        TS<A_RHIResourceView> _OpaqueVisibilitySRV;
        TS<A_RHIResourceView> _OpaqueVisibilityUAV;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetOpaqueVisibilityBuffer() const noexcept
        {
            return _OpaqueVisibilityBuffer;
        }
        ABYTEK_FORCE_INLINE const auto& GetOpaqueVisibilitySRV() const noexcept
        {
            return _OpaqueVisibilitySRV;
        }
        ABYTEK_FORCE_INLINE const auto& GetOpaqueVisibilityUAV() const noexcept
        {
            return _OpaqueVisibilityUAV;
        }
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_SRPRenderView, A_RenderView);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RenderViewBuildParams& BuildParams
        ) override;
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    protected:
        void OnBeginFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnEndFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void BeginOpaqueVisibilityBuffer(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndOpaqueVisibilityBuffer(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        void ClearOpaqueVisibilityBuffer(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}
