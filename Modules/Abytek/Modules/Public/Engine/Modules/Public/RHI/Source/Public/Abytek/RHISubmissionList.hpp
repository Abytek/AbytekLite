#pragma once

#include "Abytek/RHISubmissionItemContainer.hpp"
#include "Abytek/RHISubmissionListOrder.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API A_RHISubmissionList : public A_RHISubmissionItemContainer
    {
    private:
        E_RHISubmissionListOrder _Order = E_RHISubmissionListOrder::DEFAULT;

    public:
        ABYTEK_FORCE_INLINE auto GetOrder() const noexcept
        {
            return _Order;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHISubmissionList);
        virtual void Build(const TW_Valid<A_RHIProcess>& Process, E_RHISubmissionListOrder Order = E_RHISubmissionListOrder::DEFAULT);
        void Release() override;
        
    public:
        TS<A_RHISubmissionList> ForwardOrAddNew(E_RHISubmissionListOrder TargetOrder = E_RHISubmissionListOrder::DEFAULT);
    };
}
