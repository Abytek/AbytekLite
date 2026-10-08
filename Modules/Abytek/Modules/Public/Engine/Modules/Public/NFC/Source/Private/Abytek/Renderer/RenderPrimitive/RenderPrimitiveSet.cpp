#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveManager.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveProcessor.hpp"
#include "Abytek/Renderer/GPUData/GPUDataInstanceSet.hpp"


namespace Abytek
{
    TW_Valid<F_RenderPrimitiveManager> A_RenderPrimitiveSet::GetManager() const
    {
        return _Processor->GetManager();
    }
    void A_RenderPrimitiveSet::InitPrimitiveSet(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW_Valid<A_RenderPrimitiveProcessor>& Processor,
        const F_RenderPrimitiveSetConfig& Config
    )
    {
        InitMinimal(SubmissionItemContainer);
        ABYTEK_ENGINE_NFC_ASSERT(Processor) << "Invalid processor";
        _Processor = Processor;
        _Num = Config.Num;
    }
    void A_RenderPrimitiveSet::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _Num = 0;
        _Processor = {};
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void A_RenderPrimitiveSet::OnActivate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_RenderPrimitiveSet::OnDeactivate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }

    void A_RenderPrimitiveSet::Activate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        F_GPUDataInstanceSetBuildParams GPUDataInstanceSetBuildParams;
        GPUDataInstanceSetBuildParams.GPUData = _Processor->GetGPUData().Weak();
        GPUDataInstanceSetBuildParams.Num = _Num;
#ifdef ABYTEK_DEBUG_INFO
        _GPUDataInstanceSet = F_GPUDataInstanceSet::CreateAndInit_WithDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".GPUDataInstanceSet"),
#else
        _GPUDataInstanceSet = F_GPUDataInstanceSet::CreateAndInit(
#endif
            GetWorldRenderResource(),
            SubmissionItemContainer, 
            GPUDataInstanceSetBuildParams
        );
        
        _Processor->_RegisterInstanceSet(ABYTEK_WTHIS());
        
        OnActivate(SubmissionItemContainer);
        
        _Processor->OnActivatePrimitiveSet(SubmissionItemContainer, ABYTEK_STHIS());
    }
    void A_RenderPrimitiveSet::Deactivate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _Processor->OnDeactivatePrimitiveSet(SubmissionItemContainer, ABYTEK_STHIS());
        
        OnDeactivate(SubmissionItemContainer);
        
        _Processor->_UnregisterInstanceSet(ABYTEK_WTHIS());
        
        _GPUDataInstanceSet->Release(SubmissionItemContainer);
        _GPUDataInstanceSet = {};
    }
}
