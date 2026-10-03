#include "Abytek/SimplePrimitive/SRPRenderPrimitiveProcessor_Simple.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/Renderer/RenderPrimitive/Archetypes/Data_Simple.hpp"


namespace Abytek
{
    void F_SRPRenderPrimitiveProcessor_Simple::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW_Valid<F_RenderPrimitiveManager>& Manager,
        F_RenderPrimitiveProcessorId Id
    )
    {
        A_RenderPrimitiveProcessor_Simple::Init(
            SubmissionItemContainer,
            Manager,
            Id
        );
    }
    void F_SRPRenderPrimitiveProcessor_Simple::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderPrimitiveProcessor_Simple::Release(SubmissionItemContainer);
    }

    void F_SRPRenderPrimitiveProcessor_Simple::OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderPrimitiveProcessor_Simple::OnBeginUpdate(SubmissionItemContainer);
    }
    void F_SRPRenderPrimitiveProcessor_Simple::OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderPrimitiveProcessor_Simple::OnEndUpdate(SubmissionItemContainer);
    }
    void F_SRPRenderPrimitiveProcessor_Simple::OnFinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderPrimitiveProcessor_Simple::OnFinalizeFrame(SubmissionItemContainer);
    }

    void F_SRPRenderPrimitiveProcessor_Simple::InitGPUData(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const TS<F_GPUData>& GPUData
    )
    {
        RenderPrimitive::F_Data_Simple::Init(
            SubmissionItemContainer,
            GPUData,
            GetWorldRenderResource()->GetScene().Weak()
        );
    }
}
