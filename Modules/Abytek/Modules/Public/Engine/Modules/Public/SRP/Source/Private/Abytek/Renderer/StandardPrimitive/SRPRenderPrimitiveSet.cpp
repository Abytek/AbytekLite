#include "Abytek/Renderer/StandardPrimitive/SRPRenderPrimitiveSet.hpp"
#include "Abytek/Renderer/GPUData/GPUDataInstanceSet.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/Renderer/StandardPrimitive/RenderPrimitiveProcessor.hpp"


namespace Abytek
{
    void F_SRPRenderPrimitiveSet_Standard::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const TW_Valid<A_RenderPrimitiveProcessor>& Processor,
        const F_RenderPrimitiveSetConfig& Config,
        const TW_Valid<A_PrimitiveComponentRenderProxy>& PrimitiveComponentRenderProxy
    )
    {
        A_RenderPrimitiveSet_Standard::Init(
            SubmissionItemContainer,
            Processor,
            Config,
            PrimitiveComponentRenderProxy
        );
    }
    void F_SRPRenderPrimitiveSet_Standard::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderPrimitiveSet_Standard::Release(SubmissionItemContainer);
    }

    void F_SRPRenderPrimitiveSet_Standard::UploadComponents_Transform(const RenderPrimitive::F_Component_Transform* Values)
    {
        GetGPUDataInstanceSet()->UploadComponents(
            GetProcessor().FastCast<A_RenderPrimitiveProcessor_Standard>()->GetComponentIndex_Transform(),
            Values
        );
    }
    void F_SRPRenderPrimitiveSet_Standard::UploadComponents_InverseTransposeTransform(const RenderPrimitive::F_Component_Transform* Values)
    {
        GetGPUDataInstanceSet()->UploadComponents(
            GetProcessor().FastCast<A_RenderPrimitiveProcessor_Standard>()->GetComponentIndex_InverseTransposeTransform(),
            Values
        );
    }
    void F_SRPRenderPrimitiveSet_Standard::UploadComponents_GeometryAddress_ECMS(const RenderPrimitive::F_Component_GeometryAddress_ECMS* Values)
    {
        GetGPUDataInstanceSet()->UploadComponents(
            GetProcessor().FastCast<A_RenderPrimitiveProcessor_Standard>()->GetComponentIndex_GeometryAddress_ECMS(),
            Values
        );
    }
    void F_SRPRenderPrimitiveSet_Standard::UploadComponents_GeometryAddress_LOD(const RenderPrimitive::F_Component_GeometryAddress_LOD* Values)
    {
        GetGPUDataInstanceSet()->UploadComponents(
            GetProcessor().FastCast<A_RenderPrimitiveProcessor_Standard>()->GetComponentIndex_GeometryAddress_LOD(),
            Values
        );
    }
}
