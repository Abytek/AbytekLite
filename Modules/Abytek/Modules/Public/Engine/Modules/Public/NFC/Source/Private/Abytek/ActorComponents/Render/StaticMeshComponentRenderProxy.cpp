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
#include "Abytek/Renderer/StandardPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/Renderer/StandardPrimitive/RenderPrimitiveProcessor.hpp"


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
        
        UpdateWorldTransformMatrix(GetWorldTransformMatrix());
        UpdateStaticMesh(GetStaticMeshRenderProxy());
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
        
        if (ShouldUseSimplePrimitive())
        {
            if (const auto& Resource = StaticMeshRenderProxy->GetResource_ECMS())
            {
                OutPrimitiveSets.push_back(GetRenderObjectFactory()->CreatePrimitiveSet_Simple());
            }
            return;
        }
        
        if (const auto& Resource = StaticMeshRenderProxy->GetResource_ECMS())
        {
            OutPrimitiveSets.push_back(GetRenderObjectFactory()->CreatePrimitiveSet_Standard());
        }
    }
    void F_StaticMeshComponentRenderProxy::InitPrimitiveSets(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const TF_Vector<TS<A_RenderPrimitiveSet>>& PrimitiveSets
    )
    {
        if (PrimitiveSets.empty())
        {
            return;
        }
        
        if (ShouldUseSimplePrimitive())
        {
            F_RenderPrimitiveSetConfig Config;
            Config.Num = 1;
            const auto& PrimitiveSet = PrimitiveSets[0];
            PrimitiveSet.StaticCast<A_RenderPrimitiveSet_Simple>()->Init(
                SubmissionItemContainer,
                GetWorldRenderResource()->GetScene()->GetPrimitiveProcessor_Simple(),
                Config,
                ABYTEK_WTHIS()
            );
        }
        
        F_RenderPrimitiveSetConfig Config;
        Config.Num = 1;
        const auto& PrimitiveSet = PrimitiveSets[0];
        PrimitiveSet.StaticCast<A_RenderPrimitiveSet_Standard>()->Init(
            SubmissionItemContainer,
            GetWorldRenderResource()->GetScene()->GetPrimitiveProcessor_Standard(),
            Config,
            ABYTEK_WTHIS()
        );
    }

    void F_StaticMeshComponentRenderProxy::UpdateWorldTransformMatrix(const F_Matrix4x4_F32& Value)
    {
        if (ShouldUseSimplePrimitive())
        {
            for (const auto& PrimitiveSet : GetPrimitiveSets())
            {
                auto CastedPrimitiveSet = PrimitiveSet.FastCast<A_RenderPrimitiveSet_Simple>();
                CastedPrimitiveSet->UploadComponent_Transform({ Value });
                CastedPrimitiveSet->UploadComponent_InverseTransposeTransform({ Inverse(Transpose(Value)) });
            }
            return;
        }
        
        for (const auto& PrimitiveSet : GetPrimitiveSets())
        {
            auto CastedPrimitiveSet = PrimitiveSet.FastCast<A_RenderPrimitiveSet_Standard>();
            CastedPrimitiveSet->UploadComponent_Transform({ Value });
            CastedPrimitiveSet->UploadComponent_InverseTransposeTransform({ Inverse(Transpose(Value)) });
        }
    }
    void F_StaticMeshComponentRenderProxy::UpdateStaticMesh(const TS<F_StaticMeshRenderProxy>& StaticMeshRenderProxy)
    {
        if (ShouldUseSimplePrimitive())
        {
            const auto& PrimitiveSet = GetPrimitiveSets()[0];
            auto CastedPrimitiveSet = PrimitiveSet.FastCast<A_RenderPrimitiveSet_Simple>();
            
            F_RenderGeometryAddress GeometryAddress = INVALID_RENDER_GEOMETRY_ADDRESS;
            
            if (const auto& StaticMeshResource = StaticMeshRenderProxy->GetResource_ECMS())
            {
                GeometryAddress = F_RenderGeometryAddress::From(StaticMeshResource->GeometryAllocation);
            }
        
            CastedPrimitiveSet->UploadComponent_GeometryAddress_ECMS({ GeometryAddress });
            return;
        }
        
        const auto& PrimitiveSet = GetPrimitiveSets()[0];
        auto CastedPrimitiveSet = PrimitiveSet.FastCast<A_RenderPrimitiveSet_Standard>();
            
        F_RenderGeometryAddress GeometryAddress = INVALID_RENDER_GEOMETRY_ADDRESS;
            
        if (const auto& StaticMeshResource = StaticMeshRenderProxy->GetResource_ECMS())
        {
            GeometryAddress = F_RenderGeometryAddress::From(StaticMeshResource->GeometryAllocation);
        }
        
        CastedPrimitiveSet->UploadComponent_GeometryAddress_ECMS({ GeometryAddress });
    }

    B8 F_StaticMeshComponentRenderProxy::ShouldUseSimplePrimitive() const
    {
        return false;
    }
}
