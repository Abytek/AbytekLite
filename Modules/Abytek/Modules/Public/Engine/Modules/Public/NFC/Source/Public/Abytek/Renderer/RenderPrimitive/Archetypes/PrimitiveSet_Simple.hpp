#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/Renderer/RenderPrimitive/Archetypes/Data_Simple.hpp"


namespace Abytek
{
    class F_StaticMeshComponentRenderProxy;
    class A_RenderPrimitiveProcessor_Simple;

    class ABYTEK_ENGINE_NFC_API A_RenderPrimitiveSet_Simple : public A_RenderPrimitiveSet
    {
    private:
        TW<F_StaticMeshComponentRenderProxy> _StaticMeshComponentRenderProxy;
        U32 _StaticMeshResourceIndex = ~U32(0);
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetStaticMeshComponentRenderProxy() const noexcept
        {
            return _StaticMeshComponentRenderProxy;
        }
        ABYTEK_FORCE_INLINE auto GetStaticMeshResourceIndex() const noexcept
        {
            return _StaticMeshResourceIndex;
        }
        
    public:
        ABYTEK_RENDER_OBJECT(A_RenderPrimitiveSet_Simple, A_RenderPrimitiveSet);
        
    public:
        virtual void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TW_Valid<A_RenderPrimitiveProcessor>& Processor,
            const F_RenderPrimitiveSetConfig& Config,
            const TW_Valid<F_StaticMeshComponentRenderProxy>& StaticMeshComponentRenderProxy,
            U32 StaticMeshResourceIndex
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        virtual void UploadComponents_Transform(const RenderPrimitive::F_Component_Transform* Values) = 0;
        virtual void UploadComponents_InverseTransposeTransform(const RenderPrimitive::F_Component_Transform* Values) = 0;
        virtual void UploadComponents_GeometryAddress_ECMS(const RenderPrimitive::F_Component_GeometryAddress_ECMS* Values) = 0;
        virtual void UploadComponents_GeometryAllocationStructure_ECMS(const RenderPrimitive::F_Component_GeometryAllocationStructure_ECMS* Values) = 0;
        void UploadComponent_Transform(const RenderPrimitive::F_Component_Transform& Value);
        void UploadComponent_InverseTransposeTransform(const RenderPrimitive::F_Component_Transform& Value);
        void UploadComponent_GeometryAddress_ECMS(const RenderPrimitive::F_Component_GeometryAddress_ECMS& Value);
        void UploadComponent_GeometryAllocationStructure_ECMS(const RenderPrimitive::F_Component_GeometryAllocationStructure_ECMS& Value);
    };
}
