#include "Abytek/SimplePrimitive/SRPRenderPrimitiveSet_Simple.hpp"
#include "Abytek/Renderer/GPUData/GPUDataInstanceSet.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/Renderer/RenderPrimitive/Archetypes/Processor_Simple.hpp"


namespace Abytek
{
    void F_SRPRenderPrimitiveSet_Simple::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const TW_Valid<A_RenderPrimitiveProcessor>& Processor,
        const F_RenderPrimitiveSetConfig& Config,
        const TW_Valid<F_StaticMeshComponentRenderProxy>& StaticMeshComponentRenderProxy
    )
    {
        A_RenderPrimitiveSet_Simple::Init(
            SubmissionItemContainer,
            Processor,
            Config,
            StaticMeshComponentRenderProxy
        );
    }
    void F_SRPRenderPrimitiveSet_Simple::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderPrimitiveSet_Simple::Release(SubmissionItemContainer);
    }

    void F_SRPRenderPrimitiveSet_Simple::UploadComponents_Transform(const RenderPrimitive::F_Component_Transform* Values)
    {
        GetGPUDataInstanceSet()->UploadComponents(
            GetProcessor().FastCast<A_RenderPrimitiveProcessor_Simple>()->GetComponentIndex_Transform(),
            Values
        );
    }
    void F_SRPRenderPrimitiveSet_Simple::UploadComponents_InverseTransposeTransform(const RenderPrimitive::F_Component_Transform* Values)
    {
        GetGPUDataInstanceSet()->UploadComponents(
            GetProcessor().FastCast<A_RenderPrimitiveProcessor_Simple>()->GetComponentIndex_InverseTransposeTransform(),
            Values
        );
    }
    void F_SRPRenderPrimitiveSet_Simple::UploadComponents_GeometryAddress_ECMS(const RenderPrimitive::F_Component_GeometryAddress_ECMS* Values)
    {
        GetGPUDataInstanceSet()->UploadComponents(
            GetProcessor().FastCast<A_RenderPrimitiveProcessor_Simple>()->GetComponentIndex_GeometryAddress_ECMS(),
            Values
        );
    }
    void F_SRPRenderPrimitiveSet_Simple::UploadComponents_GeometryAllocationStructure_ECMS(const RenderPrimitive::F_Component_GeometryAllocationStructure_ECMS* Values)
    {
        GetGPUDataInstanceSet()->UploadComponents(
            GetProcessor().FastCast<A_RenderPrimitiveProcessor_Simple>()->GetComponentIndex_GeometryAllocationStructure_ECMS(),
            Values
        );
    }
}
