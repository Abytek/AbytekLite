#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveManager.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveProcessor.hpp"


namespace Abytek
{
    void F_RenderPrimitiveManager::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderPrimitiveManagerBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Scene = BuildParams.Scene;
    }
    void F_RenderPrimitiveManager::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        for (const auto& Proccessor : _Processors)
        {
            Proccessor->Release(SubmissionItemContainer);
        }
        _Processors = {};
        
        _Scene = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void F_RenderPrimitiveManager::BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        for (const auto& Proccessor : _Processors)
        {
            Proccessor->BeginUpdate(SubmissionItemContainer);
        }
    }
    void F_RenderPrimitiveManager::EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        for (const auto& Proccessor : _Processors)
        {
            Proccessor->EndUpdate(SubmissionItemContainer);
        }
    }
    void F_RenderPrimitiveManager::FinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        for (auto It = _Processors.rbegin(); It != _Processors.rend(); ++It)
        {
            const auto& Proccessor = *It;
            Proccessor->FinalizeFrame(SubmissionItemContainer);
        }
    }
}
