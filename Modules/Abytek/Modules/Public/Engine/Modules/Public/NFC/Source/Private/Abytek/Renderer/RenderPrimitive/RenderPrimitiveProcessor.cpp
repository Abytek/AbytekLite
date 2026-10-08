#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveProcessor.hpp"
#include "Abytek/Renderer/GPUData/GPUDataInstanceSet.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveManager.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"


namespace Abytek
{
    void A_RenderPrimitiveProcessor::InitPrimitiveProcessor(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW_Valid<F_RenderPrimitiveManager>& Manager,
        F_RenderPrimitiveProcessorId Id
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Manager = Manager;
        _Id = Id;
        
        _GPUData = F_GPUData::Create(GetWorldRenderResource());
#ifdef ABYTEK_DEBUG_INFO
        _GPUData->SetDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".GPUData")
        );
#endif
        InitGPUData(SubmissionItemContainer, _GPUData);
    }
    void A_RenderPrimitiveProcessor::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _GPUData->Release(SubmissionItemContainer);
        _GPUData = {};
        
        _Id = INVALID_RENDER_PRIMITIVE_PROCESSOR_ID;
        _Manager = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void A_RenderPrimitiveProcessor::_RegisterInstanceSet(const TW_Valid<A_RenderPrimitiveSet>& PrimitiveSet)
    {
        _CriticalSection(
            [this, &PrimitiveSet]
            {
                PrimitiveSet->_Index = static_cast<U32>(_PrimitiveSets.size());
                _PrimitiveSets.push_back(PrimitiveSet);
            }
        );
    }
    void A_RenderPrimitiveProcessor::_UnregisterInstanceSet(const TW_Valid<A_RenderPrimitiveSet>& PrimitiveSet)
    {
        _CriticalSection(
            [this, &PrimitiveSet]
            {
                _PrimitiveSets.back()->_Index = PrimitiveSet->_Index;
                std::swap(
                    _PrimitiveSets[PrimitiveSet->_Index],
                    _PrimitiveSets.back()
                );
                _PrimitiveSets.pop_back();
            }
        );
    }

    void A_RenderPrimitiveProcessor::OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_RenderPrimitiveProcessor::OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_RenderPrimitiveProcessor::OnFinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }

    void A_RenderPrimitiveProcessor::BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _CriticalSection(
            [this, &SubmissionItemContainer]
            {
                _GPUData->BeginUpdate(SubmissionItemContainer);
                OnBeginUpdate(SubmissionItemContainer);
            }
        );
    }
    void A_RenderPrimitiveProcessor::EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _CriticalSection(
            [this, &SubmissionItemContainer]
            {
                OnEndUpdate(SubmissionItemContainer);
                _GPUData->EndUpdate(SubmissionItemContainer);
            }
        );
    }
    void A_RenderPrimitiveProcessor::FinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _CriticalSection(
            [this, &SubmissionItemContainer]
            {
                OnFinalizeFrame(SubmissionItemContainer);
                _GPUData->FinalizeFrame(SubmissionItemContainer);
            }
        );
    }

    void A_RenderPrimitiveProcessor::OnActivatePrimitiveSet(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const TS<A_RenderPrimitiveSet>& PrimitiveSet)
    {
    }
    void A_RenderPrimitiveProcessor::OnDeactivatePrimitiveSet(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const TS<A_RenderPrimitiveSet>& PrimitiveSet)
    {
    }
}
