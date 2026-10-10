#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/Renderer/SRPVisibilityBuffer.hpp"
#include "Abytek/Renderer/ECMS/SRPInstancedMeshletBuffer.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPRenderView final : public A_RenderView
    {
    private:
        SRP::VisibilityBuffer::F_OpaqueInstance _OpaqueVisibilityBuffer;
        SRP::ECMS::F_InstancedMeshletBuffer _InstancedMeshletBuffer_ECMS;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetOpaqueVisibilityBuffer() const noexcept
        {
            return _OpaqueVisibilityBuffer;
        }
        ABYTEK_FORCE_INLINE const auto& GetInstancedMeshletBuffer_ECMS() const noexcept
        {
            return _InstancedMeshletBuffer_ECMS;
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
        void ClearOpaqueVisibilityBuffer(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}
