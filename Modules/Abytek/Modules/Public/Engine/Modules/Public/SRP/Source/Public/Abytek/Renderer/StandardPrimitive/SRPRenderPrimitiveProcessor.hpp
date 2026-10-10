#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/StandardPrimitive/RenderPrimitiveProcessor.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_SRPRenderPrimitiveProcessor_Standard : public A_RenderPrimitiveProcessor_Standard
    {
    private:
        
    public:
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_SRPRenderPrimitiveProcessor_Standard, A_RenderPrimitiveProcessor_Standard);
        
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
