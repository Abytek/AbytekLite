#include "Abytek/Renderer/StandardPrimitive/SRPRenderPrimitiveProcessor.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/Renderer/StandardPrimitive/RenderPrimitiveData.hpp"


namespace Abytek
{
    void F_SRPRenderPrimitiveProcessor_Standard::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW_Valid<F_RenderPrimitiveManager>& Manager,
        F_RenderPrimitiveProcessorId Id
    )
    {
        A_RenderPrimitiveProcessor_Standard::Init(
            SubmissionItemContainer,
            Manager,
            Id
        );
    }
    void F_SRPRenderPrimitiveProcessor_Standard::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderPrimitiveProcessor_Standard::Release(SubmissionItemContainer);
    }

    void F_SRPRenderPrimitiveProcessor_Standard::OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderPrimitiveProcessor_Standard::OnBeginUpdate(SubmissionItemContainer);
    }
    void F_SRPRenderPrimitiveProcessor_Standard::OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderPrimitiveProcessor_Standard::OnEndUpdate(SubmissionItemContainer);
    }
    void F_SRPRenderPrimitiveProcessor_Standard::OnFinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderPrimitiveProcessor_Standard::OnFinalizeFrame(SubmissionItemContainer);
    }

    void F_SRPRenderPrimitiveProcessor_Standard::InitGPUData(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const TS<F_GPUData>& GPUData
    )
    {
        RenderPrimitive::F_Data_Standard::Init(
            SubmissionItemContainer,
            GPUData,
            GetWorldRenderResource()->GetScene().Weak()
        );
    }
}
