#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/RenderPrimitive/Archetypes/PrimitiveSet_Simple.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_SRPRenderPrimitiveSet_Simple : public A_RenderPrimitiveSet_Simple
    {
    private:
        
    public:
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_SRPRenderPrimitiveSet_Simple, A_RenderPrimitiveSet_Simple);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TW_Valid<A_RenderPrimitiveProcessor>& Processor,
            const F_RenderPrimitiveSetConfig& Config,
            const TW_Valid<F_StaticMeshComponentRenderProxy>& StaticMeshComponentRenderProxy
        ) override;
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void UploadComponents_Transform(const RenderPrimitive::F_Component_Transform* Values) override;
        void UploadComponents_InverseTransposeTransform(const RenderPrimitive::F_Component_Transform* Values) override;
        void UploadComponents_GeometryAddress_ECMS(const RenderPrimitive::F_Component_GeometryAddress_ECMS* Values) override;
        void UploadComponents_GeometryAllocationStructure_ECMS(const RenderPrimitive::F_Component_GeometryAllocationStructure_ECMS* Values) override;
    };
}
