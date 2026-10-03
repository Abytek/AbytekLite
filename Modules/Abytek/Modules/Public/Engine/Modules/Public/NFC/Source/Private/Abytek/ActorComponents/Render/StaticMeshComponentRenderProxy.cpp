#include "Abytek/ActorComponents/Render/StaticMeshComponentRenderProxy.hpp"
#include "Abytek/Assets/Render/StaticMeshRenderProxy.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/Renderer/Renderer.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderer/RenderPrimitive/Archetypes/Processor_Simple.hpp"
#include "Abytek/Renderer/RenderPrimitive/Archetypes/PrimitiveSet_Simple.hpp"


namespace Abytek
{
    F_StaticMeshComponentRenderProxy::F_StaticMeshComponentRenderProxy(const TW_Valid<F_StaticMeshComponent>& Owner) :
        A_PrimitiveComponentRenderProxy(Owner)
    {
    }
    F_StaticMeshComponentRenderProxy::~F_StaticMeshComponentRenderProxy()
    {
    }

    void F_StaticMeshComponentRenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_PrimitiveComponentRenderProxy::OnCreateRenderState_RenderTask(SubmissionItemContainer);
        
        UpdateWorldTransformMatrix_Simple(GetWorldTransformMatrix());
        UpdateStaticMesh_Simple(GetStaticMeshRenderProxy());
    }
    void F_StaticMeshComponentRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _StaticMeshRenderProxy = {};
        A_PrimitiveComponentRenderProxy::OnDestroyRenderState_RenderTask(SubmissionItemContainer);
    }

    void F_StaticMeshComponentRenderProxy::CreatePrimitiveSets(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        TF_Vector<TS<A_RenderPrimitiveSet>>& OutPrimitiveSets
    )
    {
        auto StaticMeshRenderProxy = GetStaticMeshRenderProxy();
        ABYTEK_ENGINE_NFC_ASSERT(StaticMeshRenderProxy->GetDataType() == E_StaticMeshDataType::ECMS);
        for (const auto& Resource : StaticMeshRenderProxy->GetResourceList_ECMS())
        {
            OutPrimitiveSets.push_back(GetRenderObjectFactory()->CreatePrimitiveSet_Simple());
        }
    }
    void F_StaticMeshComponentRenderProxy::InitPrimitiveSets(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const TF_Vector<TS<A_RenderPrimitiveSet>>& PrimitiveSets
    )
    {
        F_RenderPrimitiveSetConfig Config;
        Config.Num = 1;
        for (U32 Idx = 0; Idx < PrimitiveSets.size(); ++Idx)
        {
            const auto& PrimitiveSet = PrimitiveSets[Idx];
            PrimitiveSet.StaticCast<A_RenderPrimitiveSet_Simple>()->Init(
                SubmissionItemContainer,
                GetWorldRenderResource()->GetScene()->GetPrimitiveProcessor_Simple(),
                Config,
                ABYTEK_WTHIS(),
                Idx
            );
        }
    }

    void F_StaticMeshComponentRenderProxy::UpdateWorldTransformMatrix_Simple(const F_Matrix4x4_F32& Value)
    {
        for (const auto& PrimitiveSet : GetPrimitiveSets())
        {
            auto CastedPrimitiveSet = PrimitiveSet.FastCast<A_RenderPrimitiveSet_Simple>();
            CastedPrimitiveSet->UploadComponent_Transform({ Value });
            CastedPrimitiveSet->UploadComponent_InverseTransposeTransform({ Inverse(Transpose(Value)) });
        }
    }
    void F_StaticMeshComponentRenderProxy::UpdateStaticMesh_Simple(const TS<F_StaticMeshRenderProxy>& StaticMeshRenderProxy)
    {
        const auto& StaticMeshResources = StaticMeshRenderProxy->GetResourceList_ECMS();
        U32 NumResources = static_cast<U32>(StaticMeshResources.size());
        for (U32 Idx = 0; Idx < NumResources; ++Idx)
        {
            const auto& StaticMeshResource = StaticMeshResources[Idx];
            const auto& PrimitiveSet = GetPrimitiveSets()[Idx];
            auto CastedPrimitiveSet = PrimitiveSet.FastCast<A_RenderPrimitiveSet_Simple>();
            CastedPrimitiveSet->UploadComponent_GeometryAddress_ECMS({ F_RenderGeometryAddress::From(StaticMeshResource.GeometryAllocation) });
            CastedPrimitiveSet->UploadComponent_GeometryAllocationStructure_ECMS({ StaticMeshResource.GeometryAllocationStructure });
        }
    }
}
