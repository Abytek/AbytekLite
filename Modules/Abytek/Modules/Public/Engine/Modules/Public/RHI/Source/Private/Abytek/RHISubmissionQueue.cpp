#include "Abytek/RHISubmissionQueue.hpp"
#include "Abytek/RHIProcess.hpp"
#include "Abytek/DirectX12/RHISubmissionQueue.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHISubmissionQueue)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHISubmissionQueue>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    void A_RHISubmissionQueue::Build(const TW_Valid<A_RHIProcess>& Process)
    {
        BuildSubmissionItemContainer(Process);
    }   
    void A_RHISubmissionQueue::Release()
    {
        A_RHISubmissionItemContainer::Release();
    }

    TS_Unmanaged<F_TaskPromise> A_RHISubmissionQueue::Flush(E_RHIProcessFlushFlag Flags)
    {
        auto Process = GetProcess();
        for (const auto& Item : *this)
        {
            Process->AddSubmissionItem(Item);
        }
        Clear();
        return Process->Flush(Flags);
    }
}
