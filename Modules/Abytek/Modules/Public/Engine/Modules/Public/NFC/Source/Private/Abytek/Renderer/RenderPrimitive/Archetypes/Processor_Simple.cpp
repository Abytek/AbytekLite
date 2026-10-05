#include "Abytek/Renderer/RenderPrimitive/Archetypes/Processor_Simple.hpp"
#include "Abytek/Renderer/RenderPrimitive/Archetypes/Data_Simple.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"


namespace Abytek
{
    void A_RenderPrimitiveProcessor_Simple::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW_Valid<F_RenderPrimitiveManager>& Manager,
        F_RenderPrimitiveProcessorId Id
    )
    {
        InitPrimitiveProcessor(
            SubmissionItemContainer,
            Manager,
            Id
        );
        
        auto GPUData = GetGPUData();
        _ComponentIndex_Transform = GPUData->GetComponentTypeIndex<RenderPrimitive::F_Component_Transform>();
        _ComponentIndex_InverseTransposeTransform = GPUData->GetComponentTypeIndex<RenderPrimitive::F_Component_InverseTransposeTransform>();
        _ComponentIndex_GeometryAddress_ECMS = GPUData->GetComponentTypeIndex<RenderPrimitive::F_Component_GeometryAddress_ECMS>();
        _ComponentIndex_GeometryAddress_LOD = GPUData->GetComponentTypeIndex<RenderPrimitive::F_Component_GeometryAddress_LOD>();
    }
    void A_RenderPrimitiveProcessor_Simple::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _ComponentIndex_GeometryAddress_LOD = ~U32(0);
        _ComponentIndex_GeometryAddress_ECMS = ~U32(0);
        _ComponentIndex_InverseTransposeTransform = ~U32(0);
        _ComponentIndex_Transform = ~U32(0);
        A_RenderPrimitiveProcessor::Release(SubmissionItemContainer);
    }

    void A_RenderPrimitiveProcessor_Simple::OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_RenderPrimitiveProcessor_Simple::OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_RenderPrimitiveProcessor_Simple::OnFinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
}
