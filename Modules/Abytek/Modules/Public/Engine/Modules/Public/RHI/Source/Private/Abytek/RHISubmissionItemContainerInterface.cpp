#include "Abytek/RHISubmissionItemContainerInterface.hpp"
#include "Abytek/RHISubmissionItem.hpp"
#include "Abytek/RHISubmissionList.hpp"


namespace Abytek
{
    void I_RHISubmissionItemContainer::AddFrontSubmissionList(const TS<A_RHISubmissionList>& SubmissionList)
    {
        SubmissionList->OnAddItemsBefore(ABYTEK_WTHIS());
        OnAddFrontSubmissionList(SubmissionList);
        SubmissionList->OnAddItemsAfter(ABYTEK_WTHIS());
    }
    void I_RHISubmissionItemContainer::AddBackSubmissionItem(const TS<A_RHISubmissionItem>& SubmissionItem)
    {
        SubmissionItem->OnAddItemsBefore(ABYTEK_WTHIS());
        OnAddBackSubmissionItem(SubmissionItem);
        SubmissionItem->OnAddItemsAfter(ABYTEK_WTHIS());
    }

    TS<A_RHISubmissionList> I_RHISubmissionItemContainer::AddChild(E_RHISubmissionListOrder Order, const F_DebugName& DebugName)
    {
        return OnAddChild(Order, DebugName);
    }
    TS<A_RHISubmissionList> I_RHISubmissionItemContainer::AddChild(const F_DebugName& DebugName)
    {
        return AddChild(E_RHISubmissionListOrder::DEFAULT, DebugName);
    }
}
