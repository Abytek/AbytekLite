#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"
#include "Abytek/Task.hpp"


namespace Abytek
{
    class ABYTEK_BASE_TASK_API A_TaskCondition
    {
    protected:
        A_TaskCondition();

    public:
        virtual ~A_TaskCondition();

    public:
        virtual B8 ShouldContinue() = 0;
    };

    class ABYTEK_BASE_TASK_API F_TaskWaitForSeconds : public A_TaskCondition
    {
    private:
        boost::chrono::time_point<boost::chrono::steady_clock> _StartTime;
        float _TargetSeconds = 0;

    public:
        ABYTEK_FORCE_INLINE float GetTargetSeconds() const noexcept
        {
            return _TargetSeconds;
        }
        
    public:
        F_TaskWaitForSeconds(float TargetSeconds);
        virtual ~F_TaskWaitForSeconds() override;

    public:
        virtual B8 ShouldContinue() override;
    };

    class ABYTEK_BASE_TASK_API F_TaskWaitForPromise : public A_TaskCondition
    {
    private:
        TS_Unmanaged<F_TaskPromise> _Promise;

    public:
        ABYTEK_FORCE_INLINE const auto& GetPromise() const noexcept
        {
            return _Promise;
        }
        
    public:
        F_TaskWaitForPromise(const TS_ValidUnmanaged<F_TaskPromise>& Promise);
        virtual ~F_TaskWaitForPromise() override;

    public:
        virtual B8 ShouldContinue() override;
    };
}