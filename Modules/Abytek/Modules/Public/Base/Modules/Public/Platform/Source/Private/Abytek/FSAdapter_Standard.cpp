#include "Abytek/FSAdapter_Standard.hpp"
#include "Abytek/Windows/FSAdapter_Standard.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_FSAdapter_Standard);
    
    A_FSAdapter_Standard::A_FSAdapter_Standard()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    A_FSAdapter_Standard::~A_FSAdapter_Standard()
    {
    }

    TU<A_FSAdapter_Standard> A_FSAdapter_Standard::Create()
    {
#ifdef ABYTEK_PLATFORM_WINDOWS
        return TU<F_WindowsFSAdapter_Standard>()();
#endif
        return {};
    }
}
