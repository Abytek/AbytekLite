#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    struct ABYTEK_ENGINE_CORE_API F_HighLevelCookRange : A_Object
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_HighLevelCookRange);
        
    public:
        static F_Name GetBeginFunctionName();
        static F_Name GetEndFunctionName();
        static F_Name GetTaskTag();
        
    private:
        F_YieldSynchronizationSection _SynchronizationSection;
        TF_ConcurrentQueue<TF_Function<void()>> _Queue;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetQueue() const noexcept
        {
            return _Queue;
        }
        
    public:
        F_HighLevelCookRange();
        ~F_HighLevelCookRange() override;
        
    protected:
        void Begin();
        void End();
        
    public:
        static void EnqueueCommand(TF_Function<void()>&& Command);
        template<typename __F_Callback>
        static void Ensure(__F_Callback&& Callback)
        {
            GetInstance()->_SynchronizationSection.Ensure(
                ABYTEK_FORWARD(Callback)  
            );
        }
    };
}
#endif

#define ABYTEK_CHECK_HIGH_LEVEL_COOK_RANGE() ABYTEK_CHECK_TASK_TAG(Abytek::F_HighLevelCookRange::GetTaskTag())
#define ABYTEK_CHECK_NOT_HIGH_LEVEL_COOK_RANGE() ABYTEK_CHECK_NOT_TASK_TAG(Abytek::F_HighLevelCookRange::GetTaskTag())