#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/Renderer/StandardPrimitive/RenderPrimitiveData.hpp"


namespace Abytek
{
    class A_StandardRenderPrimitiveProcessor;
    class A_PrimitiveComponentRenderProxy;

    class ABYTEK_ENGINE_NFC_API A_RenderPrimitiveSet_Standard : public A_RenderPrimitiveSet
    {
    private:
        TW<A_PrimitiveComponentRenderProxy> _PrimitiveComponentRenderProxy;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPrimitiveComponentRenderProxy() const noexcept
        {
            return _PrimitiveComponentRenderProxy;
        }
        
    public:
        ABYTEK_RENDER_OBJECT(A_RenderPrimitiveSet_Standard, A_RenderPrimitiveSet);
        
    public:
        virtual void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TW_Valid<A_RenderPrimitiveProcessor>& Processor,
            const F_RenderPrimitiveSetConfig& Config,
            const TW_Valid<A_PrimitiveComponentRenderProxy>& PrimitiveComponentRenderProxy
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        virtual void UploadComponents_Transform(const RenderPrimitive::F_Component_Transform* Values) {}
        virtual void UploadComponents_InverseTransposeTransform(const RenderPrimitive::F_Component_Transform* Values) {}
        virtual void UploadComponents_GeometryAddress_ECMS(const RenderPrimitive::F_Component_GeometryAddress_ECMS* Values) {}
        virtual void UploadComponents_GeometryAddress_LOD(const RenderPrimitive::F_Component_GeometryAddress_LOD* Values) {}
        void UploadComponent_Transform(const RenderPrimitive::F_Component_Transform& Value);
        void UploadComponent_InverseTransposeTransform(const RenderPrimitive::F_Component_Transform& Value);
        void UploadComponent_GeometryAddress_ECMS(const RenderPrimitive::F_Component_GeometryAddress_ECMS& Value);
        void UploadComponent_GeometryAddress_LOD(const RenderPrimitive::F_Component_GeometryAddress_LOD& Value);
    };
}
