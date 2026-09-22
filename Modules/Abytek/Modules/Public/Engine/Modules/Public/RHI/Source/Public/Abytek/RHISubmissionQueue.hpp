#pragma once

#include "Abytek/RHISubmissionItemContainer.hpp"
#include "Abytek/RHIProcessFlushFlag.hpp"


namespace Abytek
{
    class A_RHIViewport;
    
    class ABYTEK_ENGINE_RHI_API A_RHISubmissionQueue : public A_RHISubmissionItemContainer
    {
    private:

    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHISubmissionQueue);
        virtual void Build(const TW_Valid<A_RHIProcess>& Process);
        void Release() override;
        
    public:
        TS_Unmanaged<F_TaskPromise> Flush(E_RHIProcessFlushFlag Flags = E_RHIProcessFlushFlag::DEFAULT);
    };
}
