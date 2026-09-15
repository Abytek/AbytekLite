#include "Abytek/DirectX12/RHISubmissionList.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHISubmissionList::Build(const TW_Valid<A_RHIProcess>& Process, E_RHISubmissionListOrder Order)
    {
        A_RHISubmissionList::Build(Process, Order);
        A_DirectX12RHISubmissionListExtension::Build();
    }
    void F_DirectX12RHISubmissionList::Release()
    {
        A_DirectX12RHISubmissionListExtension::Release();
        A_RHISubmissionList::Release();
    }
}
#endif