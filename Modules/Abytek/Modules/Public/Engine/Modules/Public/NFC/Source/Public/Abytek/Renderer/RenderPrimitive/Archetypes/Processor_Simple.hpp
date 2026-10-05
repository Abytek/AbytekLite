#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveProcessor.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API A_RenderPrimitiveProcessor_Simple : public A_RenderPrimitiveProcessor
    {
    private:
        U32 _ComponentIndex_Transform = ~U32(0);
        U32 _ComponentIndex_InverseTransposeTransform = ~U32(0);
        U32 _ComponentIndex_GeometryAddress_ECMS = ~U32(0);
        U32 _ComponentIndex_GeometryAddress_LOD = ~U32(0);
        
    public:
        ABYTEK_FORCE_INLINE auto GetComponentIndex_Transform() const noexcept
        {
            return _ComponentIndex_Transform;
        }
        ABYTEK_FORCE_INLINE auto GetComponentIndex_InverseTransposeTransform() const noexcept
        {
            return _ComponentIndex_InverseTransposeTransform;
        }
        ABYTEK_FORCE_INLINE auto GetComponentIndex_GeometryAddress_ECMS() const noexcept
        {
            return _ComponentIndex_GeometryAddress_ECMS;
        }
        ABYTEK_FORCE_INLINE auto GetComponentIndex_GeometryAddress_LOD() const noexcept
        {
            return _ComponentIndex_GeometryAddress_LOD;
        }
        
    public:
        ABYTEK_RENDER_OBJECT(A_RenderPrimitiveProcessor_Simple, A_RenderPrimitiveProcessor);
        
    public:
        virtual void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TW_Valid<F_RenderPrimitiveManager>& Manager,
            F_RenderPrimitiveProcessorId Id
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnFinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
