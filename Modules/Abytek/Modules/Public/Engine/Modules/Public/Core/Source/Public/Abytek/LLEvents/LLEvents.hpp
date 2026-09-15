#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


namespace Abytek
{
    namespace LLEvents
    {
        using F_OnTick = TF_Event<void()>;
    }  
    struct F_LLEvents
    {
        LLEvents::F_OnTick OnTick;
    };
    
    class ABYTEK_ENGINE_CORE_API A_LLEvents : public A_Object, public F_LLEvents
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_LLEvents);

        AU32 _LiveCounter = 0;

    public:
        ABYTEK_FORCE_INLINE U32 GetLiveCounter() const noexcept
        {
            return _LiveCounter.load(boost::memory_order_acquire);
        }
        ABYTEK_FORCE_INLINE U32 IncreaseLiveCounter(U32 Value) noexcept
        {
            return _LiveCounter.fetch_add(Value, boost::memory_order_acq_rel);
        }
        ABYTEK_FORCE_INLINE U32 DecreaseLiveCounter(U32 Value) noexcept
        {
            return _LiveCounter.fetch_sub(Value, boost::memory_order_acq_rel);
        }
        
    public:
        A_LLEvents();
        ~A_LLEvents() override;

    private:
        B8 _NewFrame();
        
    public:
        B8 Tick();

    public:
        virtual void Start();
        virtual void FinalizeFrame();
        
    public:
        static TU<A_LLEvents> Create();
    };
}