#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIProcess.hpp"


namespace Abytek
{
    template<class __F>
    struct TF_RHICompileDataHandle
    {
        mutable __F* RawPtr = nullptr;
        mutable U32 ProcessCounter = ~U32(0);
        
        ABYTEK_FORCE_INLINE TF_RHICompileDataHandle() = default;
        ABYTEK_FORCE_INLINE TF_RHICompileDataHandle(const TF_RHICompileDataHandle& X) = default;
        ABYTEK_FORCE_INLINE TF_RHICompileDataHandle& operator = (const TF_RHICompileDataHandle& X) = default;
        ABYTEK_FORCE_INLINE TF_RHICompileDataHandle(TF_RHICompileDataHandle&& X) = default;
        ABYTEK_FORCE_INLINE TF_RHICompileDataHandle& operator = (TF_RHICompileDataHandle&& X) = default;
        
        ABYTEK_FORCE_INLINE void Ensure(const TW_Valid<A_RHIProcess>& Process) const
        { 
            if (RawPtr && (ProcessCounter == Process->GetCounter()) && (ProcessCounter != ~U32(0)))
            {
                return;
            }
            RawPtr = Process->NewCompileData<__F>(ProcessCounter);
        }
        ABYTEK_FORCE_INLINE __F* Get(const TW_Valid<A_RHIProcess>& Process) const 
        {
            Ensure(Process);
            return RawPtr;
        }
    };
}