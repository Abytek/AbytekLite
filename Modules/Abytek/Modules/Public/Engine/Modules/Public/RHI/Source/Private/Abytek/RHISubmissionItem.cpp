#include "Abytek/RHISubmissionItem.hpp"
#include "Abytek/RHISubmissionItemContainerInterface.hpp"
#include "Abytek/RHIProcess.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    void A_RHISubmissionItem::BuildSubmissionItem()
    {
        BuildMinimal();
    }
    void A_RHISubmissionItem::Release()
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        _StackCaptureEventStates.clear();
        _CaptureEventState = {};
#endif
        A_RAObject::Release();
    }

    void A_RHISubmissionItem::OnAddItemsBefore(const TW_Valid<I_RHISubmissionItemContainer>& Container)
    {
    }
    void A_RHISubmissionItem::OnAddItemsAfter(const TW_Valid<I_RHISubmissionItemContainer>& Container)
    {
    }

#ifdef ABYTEK_DEBUG_INFO
    void A_RHISubmissionItem::SetDebugName(const F_DebugName& Value) noexcept
    {
        A_RAObject::SetDebugName(Value);
        _CaptureEventState.Name = Value;
    }
#endif
}
