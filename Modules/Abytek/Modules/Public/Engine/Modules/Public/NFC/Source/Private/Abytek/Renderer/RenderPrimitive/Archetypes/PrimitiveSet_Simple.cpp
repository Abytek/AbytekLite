#include "Abytek/Renderer/RenderPrimitive/Archetypes/PrimitiveSet_Simple.hpp"


namespace Abytek
{
    void A_RenderPrimitiveSet_Simple::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW_Valid<A_RenderPrimitiveProcessor>& Processor,
        const F_RenderPrimitiveSetConfig& Config,
        const TW_Valid<F_StaticMeshComponentRenderProxy>& StaticMeshComponentRenderProxy,
        U32 StaticMeshResourceIndex
    )
    {
        InitPrimitiveSet(
            SubmissionItemContainer,
            Processor,
            Config
        );
        _StaticMeshComponentRenderProxy = StaticMeshComponentRenderProxy;
        _StaticMeshResourceIndex = StaticMeshResourceIndex;
    }
    void A_RenderPrimitiveSet_Simple::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _StaticMeshResourceIndex = ~U32(0);
        _StaticMeshComponentRenderProxy = {};
        A_RenderPrimitiveSet::Release(SubmissionItemContainer);
    }

    void A_RenderPrimitiveSet_Simple::UploadComponent_Transform(const RenderPrimitive::F_Component_Transform& Value)
    {
        UploadComponents_Transform(&Value);
    }
    void A_RenderPrimitiveSet_Simple::UploadComponent_InverseTransposeTransform(const RenderPrimitive::F_Component_Transform& Value)
    {
        UploadComponents_InverseTransposeTransform(&Value);
    }
    void A_RenderPrimitiveSet_Simple::UploadComponent_GeometryAddress_ECMS(const RenderPrimitive::F_Component_GeometryAddress_ECMS& Value)
    {
        UploadComponents_GeometryAddress_ECMS(&Value);
    }
    void A_RenderPrimitiveSet_Simple::UploadComponent_GeometryAllocationStructure_ECMS(const RenderPrimitive::F_Component_GeometryAllocationStructure_ECMS& Value)
    {
        UploadComponents_GeometryAllocationStructure_ECMS(&Value);
    }
}
