#include "Abytek/DirectX12/RHISubmissionQueue.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHISubmissionQueue::Build(const TW_Valid<A_RHIProcess>& Process)
    {
        A_RHISubmissionQueue::Build(Process);
    }
    void F_DirectX12RHISubmissionQueue::Release()
    {
        A_RHISubmissionQueue::Release();
    }
}
#endif