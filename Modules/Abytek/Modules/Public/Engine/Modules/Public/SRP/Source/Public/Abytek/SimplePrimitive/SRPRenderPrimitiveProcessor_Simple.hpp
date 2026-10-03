#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/RenderPrimitive/Archetypes/Processor_Simple.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_SRPRenderPrimitiveProcessor_Simple : public A_RenderPrimitiveProcessor_Simple
    {
    private:
        
    public:
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_SRPRenderPrimitiveProcessor_Simple, A_RenderPrimitiveProcessor_Simple);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TW_Valid<F_RenderPrimitiveManager>& Manager,
            F_RenderPrimitiveProcessorId Id
        ) override;
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnFinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    protected:
        void InitGPUData(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TS<F_GPUData>& GPUData
        ) override;
    };
}
