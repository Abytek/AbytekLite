#include "Abytek/RHISubmissionItemContainer.hpp"
#include "Abytek/RHISubmissionList.hpp"
#include "Abytek/RHIProcess.hpp"
#include "Abytek/RHISubmissionQueue.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    void A_RHISubmissionItemContainer::BuildSubmissionItemContainer(const TW_Valid<A_RHIProcess>& Process)
    {
        BuildSubmissionItem();
        _Process = Process;
    }   
    void A_RHISubmissionItemContainer::Release()
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
        if (auto Process = ShareObject(_Process))
        {
            ABYTEK_ENGINE_RHI_ASSERT(Process->GetStage() == E_RHIProcessStage::COMPILE) << "Invalid moment to release submission item (too late, must be inside the compile stage)";
        }
#endif
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        _AdditionalStackCaptureEventStates.clear();
#endif
        _Items = {};
        _Process = {};
        A_RHISubmissionItem::Release();
    }

    void A_RHISubmissionItemContainer::OnAddFrontSubmissionList(const TS<A_RHISubmissionList>& SubmissionList)
    {
        ABYTEK_ENGINE_RHI_ASSERT(!SubmissionList.CheckPolymorphism<A_RHISubmissionQueue>()) << "Cannot add submission queue to any submission item container";
        for (const auto& CaptureEventState : _AdditionalStackCaptureEventStates)
        {
            SubmissionList->AddStackCaptureEventState(CaptureEventState);
        }
        _Items.insert(
            _Items.begin(),
            SubmissionList
        );
    }
    void A_RHISubmissionItemContainer::OnAddBackSubmissionItem(const TS<A_RHISubmissionItem>& SubmissionItem)
    {
        ABYTEK_ENGINE_RHI_ASSERT(!SubmissionItem.CheckPolymorphism<A_RHISubmissionQueue>()) << "Cannot add submission queue to any submission item container";
        for (const auto& CaptureEventState : _AdditionalStackCaptureEventStates)
        {
            SubmissionItem->AddStackCaptureEventState(CaptureEventState);
        }
        _Items.push_back({ SubmissionItem });
    }

    TS<A_RHISubmissionList> A_RHISubmissionItemContainer::OnAddChild(E_RHISubmissionListOrder Order, const F_DebugName& DebugName)
    {
        auto Result = RACreateAndBuildShared<A_RHISubmissionList>(_Process, Order);
#ifdef ABYTEK_DEBUG_INFO
        if (DebugName)
        {
            Result->SetDebugName(DebugName);
        }
#endif
        AddSubmissionItem(Result);
        return Result;
    }
}
