#pragma once

#include "Abytek/RHISubmissionQueue.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHISubmissionQueue : public A_RHISubmissionQueue
    {
    private:

    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHISubmissionQueue);
        void Build(const TW_Valid<A_RHIProcess>& Process) override;
        void Release() override;
    };
}
#endif