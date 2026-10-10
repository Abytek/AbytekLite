#include "Abytek/Renderer/StandardPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/ActorComponents/Render/StaticMeshComponentRenderProxy.hpp"


namespace Abytek
{
    void A_RenderPrimitiveSet_Standard::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW_Valid<A_RenderPrimitiveProcessor>& Processor,
        const F_RenderPrimitiveSetConfig& Config,
        const TW_Valid<A_PrimitiveComponentRenderProxy>& PrimitiveComponentRenderProxy
    )
    {
        InitPrimitiveSet(
            SubmissionItemContainer,
            Processor,
            Config
        );
        _PrimitiveComponentRenderProxy = PrimitiveComponentRenderProxy;
    }
    void A_RenderPrimitiveSet_Standard::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _PrimitiveComponentRenderProxy  = {};
        A_RenderPrimitiveSet::Release(SubmissionItemContainer);
    }

    void A_RenderPrimitiveSet_Standard::UploadComponent_Transform(const RenderPrimitive::F_Component_Transform& Value)
    {
        UploadComponents_Transform(&Value);
    }
    void A_RenderPrimitiveSet_Standard::UploadComponent_InverseTransposeTransform(const RenderPrimitive::F_Component_Transform& Value)
    {
        UploadComponents_InverseTransposeTransform(&Value);
    }
    void A_RenderPrimitiveSet_Standard::UploadComponent_GeometryAddress_ECMS(const RenderPrimitive::F_Component_GeometryAddress_ECMS& Value)
    {
        UploadComponents_GeometryAddress_ECMS(&Value);
    }
    void A_RenderPrimitiveSet_Standard::UploadComponent_GeometryAddress_LOD(const RenderPrimitive::F_Component_GeometryAddress_LOD& Value)
    {
        UploadComponents_GeometryAddress_LOD(&Value);
    }
}
