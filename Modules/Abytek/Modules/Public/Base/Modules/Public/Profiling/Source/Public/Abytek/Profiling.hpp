#pragma once

#include "Abytek/Base.Profiling.prerequisites.pch.hpp"


namespace Abytek
{
    ABYTEK_BASE_PROFILING_API B8 IsProfilerEnabled();
    ABYTEK_BASE_PROFILING_API void EnableProfiler();
    ABYTEK_BASE_PROFILING_API void DisableProfiler();
}

#ifdef ABYTEK_ENABLE_PROFILER
#define ABYTEK_PROFILER_THREAD() // ZoneScoped
#define ABYTEK_PROFILER_THREAD_NAMED(...) // ZoneScopedN(__VA_ARGS__)
#define ABYTEK_PROFILER_EVENT() SuppressVarShadowWarning( ZoneNamed( ___tracy_scoped_zone, Abytek::IsProfilerEnabled() ) )
#define ABYTEK_PROFILER_EVENT_NAMED(...) SuppressVarShadowWarning( ZoneNamedN( ___tracy_scoped_zone, __VA_ARGS__, Abytek::IsProfilerEnabled() ) )
#define ABYTEK_PROFILER_EVENT_SET_NAME(...) \
            if (Abytek::IsProfilerEnabled()) \
            { \
                auto Str = Abytek::ToString(__VA_ARGS__); \
                ___tracy_scoped_zone.Name(Str.data(), Str.size()); \
            }
#define ABYTEK_PROFILER_FRAME() { if (Abytek::IsProfilerEnabled()) FrameMark; }
#define ABYTEK_PROFILER_FIBER_ENTER(...) { if (Abytek::IsProfilerEnabled()) TracyFiberEnter(__VA_ARGS__); }
#define ABYTEK_PROFILER_FIBER_LEAVE() { if (Abytek::IsProfilerEnabled()) TracyFiberLeave; }
#else
#define ABYTEK_PROFILER_THREAD()
#define ABYTEK_PROFILER_THREAD_NAMED(...)
#define ABYTEK_PROFILER_EVENT()
#define ABYTEK_PROFILER_EVENT_NAMED(...)
#define ABYTEK_PROFILER_EVENT_SET_NAME(...)
#define ABYTEK_PROFILER_FRAME()
#define ABYTEK_PROFILER_FIBER_ENTER(...)
#define ABYTEK_PROFILER_FIBER_LEAVE()
#endif