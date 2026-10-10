#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/StandardPrimitive/RenderPrimitiveSet.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_SRPRenderPrimitiveSet_Standard : public A_RenderPrimitiveSet_Standard
    {
    private:
        
    public:
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_SRPRenderPrimitiveSet_Standard, A_RenderPrimitiveSet_Standard);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TW_Valid<A_RenderPrimitiveProcessor>& Processor,
            const F_RenderPrimitiveSetConfig& Config,
            const TW_Valid<A_PrimitiveComponentRenderProxy>& PrimitiveComponentRenderProxy
        ) override;
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void UploadComponents_Transform(const RenderPrimitive::F_Component_Transform* Values) override;
        void UploadComponents_InverseTransposeTransform(const RenderPrimitive::F_Component_Transform* Values) override;
        void UploadComponents_GeometryAddress_ECMS(const RenderPrimitive::F_Component_GeometryAddress_ECMS* Values) override;
        void UploadComponents_GeometryAddress_LOD(const RenderPrimitive::F_Component_GeometryAddress_LOD* Values) override;
    };
}
