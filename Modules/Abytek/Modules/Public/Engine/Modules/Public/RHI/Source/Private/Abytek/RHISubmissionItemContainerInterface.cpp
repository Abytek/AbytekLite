#include "Abytek/RHISubmissionItemContainerInterface.hpp"
#include "Abytek/RHISubmissionItem.hpp"
#include "Abytek/RHISubmissionList.hpp"


namespace Abytek
{
    void I_RHISubmissionItemContainer::AddFrontSubmissionList(const TS_Valid<A_RHISubmissionList>& SubmissionList)
    {
        SubmissionList->OnAddItemsBefore(*this);
        OnAddFrontSubmissionList(SubmissionList);
        SubmissionList->OnAddItemsAfter(*this);
    }
    void I_RHISubmissionItemContainer::AddBackSubmissionItem(const TS_Valid<A_RHISubmissionItem>& SubmissionItem)
    {
        SubmissionItem->OnAddItemsBefore(*this);
        OnAddBackSubmissionItem(SubmissionItem);
        SubmissionItem->OnAddItemsAfter(*this);
    }
}
