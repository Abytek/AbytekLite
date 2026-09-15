#pragma once

#include "Abytek/ApplicationSubsystem.hpp"


namespace Abytek
{
    class A_Ticker;
    
    class ABYTEK_ENGINE_CORE_API F_GlobalTime final : public A_ApplicationSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_GlobalTime)

    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_GlobalTime);
        
    private:
        boost::chrono::high_resolution_clock::time_point _InitialTimePoint;
        boost::chrono::high_resolution_clock::time_point _LastTimePoint;
        U64 _PassedNanoseconds = 0;
        U64 _LastDeltaNanoseconds = 0;
        F64 _LastDeltaSeconds = 0;

    public:
        ABYTEK_FORCE_INLINE const auto& GetInitialTimePoint() const noexcept
        {
            return _InitialTimePoint;
        }
        ABYTEK_FORCE_INLINE const auto& GetLastTimePoint() const noexcept
        {
            return _LastTimePoint;
        }
        ABYTEK_FORCE_INLINE U64 GetPassedNanoseconds() const noexcept
        {
            return _PassedNanoseconds;
        }
        ABYTEK_FORCE_INLINE U64 GetLastDeltaNanoseconds() const noexcept
        {
            return _LastDeltaNanoseconds;
        }
        ABYTEK_FORCE_INLINE F64 GetLastDeltaSeconds() const noexcept
        {
            return _LastDeltaSeconds;
        }
        ABYTEK_FORCE_INLINE F64 GetLastFPS() const noexcept
        {
            return 1.0 / _LastDeltaSeconds;
        }

    public:
        F_GlobalTime(const F_ProgramUnitBuildParams& BuildParams);
        ~F_GlobalTime() override;

    public:
        void NewFrame();
    };
}
