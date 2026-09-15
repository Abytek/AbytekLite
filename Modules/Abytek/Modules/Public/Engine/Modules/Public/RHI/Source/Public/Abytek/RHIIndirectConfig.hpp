#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIResourceProxy.hpp"


namespace Abytek
{
    enum class E_RHIIndirectArgumentType
    {
        NONE,
        DRAW_NON_INDEXED,
        DRAW_INDEXED,
        DISPATCH_COMPUTE,
        DISPATCH_MESH
    };
    
    struct F_RHIIndirectConfig
    {
        TS<A_RHIResource> ArgumentBuffer;
        U64 ArgumentBufferOffsetInBytes = 0;
        
        TS<A_RHIResource> CountBuffer;
        U64 CountBufferOffsetInBytes = 0;
        
        U32 MaxCount = 1;
        
        static F_RHIIndirectConfig Make(
            const TS_Valid<A_RHIResource>& ArgumentBuffer,
            U64 ArgumentBufferOffsetInBytes
        )
        {
            F_RHIIndirectConfig Result;
            Result.ArgumentBuffer = ArgumentBuffer;
            Result.ArgumentBufferOffsetInBytes = ArgumentBufferOffsetInBytes;
            return ABYTEK_MOVE(Result);
        }
        static F_RHIIndirectConfig Make(
            const TS_Valid<A_RHIResource>& ArgumentBuffer,
            U64 ArgumentBufferOffsetInBytes,
            const TS_Valid<A_RHIResource>& CountBuffer,
            U64 CountBufferOffsetInBytes,
            U32 MaxCount = 1
        )
        {
            F_RHIIndirectConfig Result;
            Result.ArgumentBuffer = ArgumentBuffer;
            Result.ArgumentBufferOffsetInBytes = ArgumentBufferOffsetInBytes;
            Result.CountBuffer = CountBuffer;
            Result.CountBufferOffsetInBytes = CountBufferOffsetInBytes;
            Result.MaxCount = MaxCount;
            return ABYTEK_MOVE(Result);
        }
    };
    
    struct F_RHIIndirectConfigProxy
    {
        TS<A_RHIResourceProxy> ArgumentBufferProxy;
        U64 ArgumentBufferOffsetInBytes = 0;
        
        TS<A_RHIResourceProxy> CountBufferProxy;
        U64 CountBufferOffsetInBytes = 0;
        
        U32 MaxCount = 1;
        
        static F_RHIIndirectConfigProxy Make(
            const TS_Valid<A_RHIResourceProxy>& ArgumentBufferProxy,
            U64 ArgumentBufferOffsetInBytes
        )
        {
            F_RHIIndirectConfigProxy Result;
            Result.ArgumentBufferProxy = ArgumentBufferProxy;
            Result.ArgumentBufferOffsetInBytes = ArgumentBufferOffsetInBytes;
            return ABYTEK_MOVE(Result);
        }
        static F_RHIIndirectConfigProxy Make(
            const TS_Valid<A_RHIResourceProxy>& ArgumentBufferProxy,
            U64 ArgumentBufferOffsetInBytes,
            const TS_Valid<A_RHIResourceProxy>& CountBufferProxy,
            U64 CountBufferOffsetInBytes,
            U32 MaxCount = 1
        )
        {
            F_RHIIndirectConfigProxy Result;
            Result.ArgumentBufferProxy = ArgumentBufferProxy;
            Result.ArgumentBufferOffsetInBytes = ArgumentBufferOffsetInBytes;
            Result.CountBufferProxy = CountBufferProxy;
            Result.CountBufferOffsetInBytes = CountBufferOffsetInBytes;
            Result.MaxCount = MaxCount;
            return ABYTEK_MOVE(Result);
        }
        
        static F_RHIIndirectConfigProxy From(const F_RHIIndirectConfig& X)
        {
            F_RHIIndirectConfigProxy Result;
            Result.ArgumentBufferProxy = X.ArgumentBuffer->GetProxy().FastCast<A_RHIResourceProxy>();
            Result.ArgumentBufferOffsetInBytes = X.ArgumentBufferOffsetInBytes;
            Result.CountBufferProxy = X.CountBuffer->GetProxy().FastCast<A_RHIResourceProxy>();
            Result.CountBufferOffsetInBytes = X.CountBufferOffsetInBytes;
            Result.MaxCount = X.MaxCount;
            return ABYTEK_MOVE(Result);
        }
    };
}