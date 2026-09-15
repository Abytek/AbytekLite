#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/RenderBase/RenderView.hpp"
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
        F_SRPRenderView(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderViewBuildParams& BuildParams);
        ~F_SRPRenderView() override;
        
    protected:
        void OnBeginFrame() override;
        void OnEndFrame() override;
        
    public:
        void BeginOpaqueVisibilityBuffer();
        void EndOpaqueVisibilityBuffer();
        
    public:
        void ClearOpaqueVisibilityBuffer(I_RHISubmissionItemContainer& SubmissionItemContainer);
    };
}
