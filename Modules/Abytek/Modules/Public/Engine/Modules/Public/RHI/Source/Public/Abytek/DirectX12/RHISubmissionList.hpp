#pragma once

#include "Abytek/RHISubmissionList.hpp"
#include "Abytek/DirectX12/RHISubmissionListExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHISubmissionList : public A_RHISubmissionList, public A_DirectX12RHISubmissionListExtension
    {
    private:

    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHISubmissionList);
        virtual void Build(const TW_Valid<A_RHIProcess>& Process, E_RHISubmissionListOrder Order) override;
        virtual void Release() override;
    };
}
#endif