#include "Abytek/GlobalTime.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_GlobalTime)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_GlobalTime"));
    }

    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_GlobalTime);

    F_GlobalTime::F_GlobalTime(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();

        _InitialTimePoint = boost::chrono::high_resolution_clock::now();
        _LastTimePoint = _InitialTimePoint;
    }
    F_GlobalTime::~F_GlobalTime()
    {
    }

    void F_GlobalTime::NewFrame()
    {
        auto CurrentTimePoint = boost::chrono::high_resolution_clock::now();
        _LastDeltaNanoseconds = boost::chrono::high_resolution_clock::duration(CurrentTimePoint - _LastTimePoint).count();
        _PassedNanoseconds += _LastDeltaNanoseconds;
        _LastDeltaSeconds = static_cast<F64>(_LastDeltaNanoseconds) / 1000000000.0;
        
        _LastTimePoint = CurrentTimePoint;
    }
}
