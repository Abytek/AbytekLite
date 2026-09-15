#include "Abytek/Profiling.hpp"


namespace Abytek
{
    namespace Internal
    {
        AB8 IsProfilerEnabled = 0;
    }
    B8 IsProfilerEnabled()
    {
        return Internal::IsProfilerEnabled.load(boost::memory_order_acquire);
    }
    void EnableProfiler()
    {
        Internal::IsProfilerEnabled.store(true, boost::memory_order_release);
    }
    void DisableProfiler()
    {
        Internal::IsProfilerEnabled.store(false, boost::memory_order_release);
    }
}
