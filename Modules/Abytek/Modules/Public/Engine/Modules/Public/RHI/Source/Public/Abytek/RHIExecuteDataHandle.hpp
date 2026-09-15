#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIProcess.hpp"


namespace Abytek
{
    template<class __F>
    struct TF_RHIExecuteDataHandle
    {
        mutable __F* RawPtr = nullptr;
        mutable U32 ProcessCounter = ~U32(0);
        
        ABYTEK_FORCE_INLINE TF_RHIExecuteDataHandle() = default;
        ABYTEK_FORCE_INLINE TF_RHIExecuteDataHandle(const TF_RHIExecuteDataHandle& X) = default;
        ABYTEK_FORCE_INLINE TF_RHIExecuteDataHandle& operator = (const TF_RHIExecuteDataHandle& X) = default;
        ABYTEK_FORCE_INLINE TF_RHIExecuteDataHandle(TF_RHIExecuteDataHandle&& X) = default;
        ABYTEK_FORCE_INLINE TF_RHIExecuteDataHandle& operator = (TF_RHIExecuteDataHandle&& X) = default;
        
        ABYTEK_FORCE_INLINE void Ensure(const TW_Valid<A_RHIProcess>& Process) const
        {
            if (RawPtr && (ProcessCounter == Process->GetCounter()) && (ProcessCounter != ~U32(0)))
            {
                return;
            } 
            RawPtr = Process->NewExecuteData<__F>(ProcessCounter);
        }
        ABYTEK_FORCE_INLINE __F* Get(const TW_Valid<A_RHIProcess>& Process) const
        {
            Ensure(Process);
            return RawPtr;
        }
    };
}