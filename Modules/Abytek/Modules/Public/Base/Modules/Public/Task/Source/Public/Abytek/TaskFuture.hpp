#pragma once

#include "Abytek/TaskPromise.hpp"


namespace Abytek
{
    template<typename __F_Data>
    class TF_TaskFuture : public F_TaskPromise
    {
    public:
        mutable __F_Data Data;

    public:
        TF_TaskFuture(U32 InitialCounter = 1) :
            F_TaskPromise(InitialCounter)
        {
        }
        TF_TaskFuture(U32 InitialCounter, __F_Data&& InData) :
            F_TaskPromise(InitialCounter),
            Data(ABYTEK_MOVE(InData))
        {
        }
        virtual ~TF_TaskFuture() override
        {
        }
        
    public:
        static TS_Unmanaged<TF_TaskFuture> MakeSynchronized(__F_Data&& InData)
        {
            auto Result = TS_Unmanaged<TF_TaskFuture>()(
                U32(0),
                ABYTEK_MOVE(InData)
            );
            return ABYTEK_MOVE(Result);
        }
        static TS_Unmanaged<TF_TaskFuture> MakeSynchronized(const __F_Data& InData)
        {
            auto Result = TS_Unmanaged<TF_TaskFuture>()(
                U32(0),
                InData
            );
            return ABYTEK_MOVE(Result);
        }
        static TS_Unmanaged<TF_TaskFuture> Make(__F_Data&& InData)
        {
            auto Result = TS_Unmanaged<TF_TaskFuture>()(
                U32(1),
                ABYTEK_MOVE(InData)
            );
            return ABYTEK_MOVE(Result);
        }
        static TS_Unmanaged<TF_TaskFuture> Make(const __F_Data& InData)
        {
            auto Result = TS_Unmanaged<TF_TaskFuture>()(
                U32(1),
                InData
            );
            return ABYTEK_MOVE(Result);
        }
        
    public:
        ABYTEK_FORCE_INLINE __F_Data& operator * () const noexcept
        {
            return Data;
        }
        ABYTEK_FORCE_INLINE __F_Data* operator -> () const noexcept
        {
            return Data;
        }
    };
}