#include "Abytek/RHISubmissionList.hpp"
#include "Abytek/DirectX12/RHISubmissionList.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHISubmissionList)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHISubmissionList>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHISubmissionList::Build(const TW_Valid<A_RHIProcess>& Process, E_RHISubmissionListOrder Order)
    {
        BuildSubmissionItemContainer(Process);
        _Order = Order;
    }   
    void A_RHISubmissionList::Release()
    {
        _Order = E_RHISubmissionListOrder::DEFAULT;
        A_RHISubmissionItemContainer::Release();
    }

    TS<A_RHISubmissionList> A_RHISubmissionList::ForwardOrAddNew(E_RHISubmissionListOrder TargetOrder)
    {
        if (_Order != TargetOrder)
        {
            auto Result = RACreateAndBuildShared<A_RHISubmissionList>(GetProcess(), TargetOrder);
            Add(Result);
            return Result;
        }
        return ABYTEK_STHIS();
    }
}
