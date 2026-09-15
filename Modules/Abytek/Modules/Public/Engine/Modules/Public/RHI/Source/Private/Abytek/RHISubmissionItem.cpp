#include "Abytek/RHISubmissionItem.hpp"
#include "Abytek/RHISubmissionItemContainerInterface.hpp"
#include "Abytek/RHIProcess.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    void A_RHISubmissionItem::Build()
    {
        A_RAObject::BuildMinimal();
    }
    void A_RHISubmissionItem::Release()
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
        _ProfilerEventColor = F_Vector3_F32::One();
#endif
        A_RAObject::Release();
    }

    void A_RHISubmissionItem::OnAddItemsBefore(I_RHISubmissionItemContainer& Container)
    {
    }
    void A_RHISubmissionItem::OnAddItemsAfter(I_RHISubmissionItemContainer& Container)
    {
    }
}
