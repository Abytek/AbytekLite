#include "Abytek/Base.prerequisites.pch.hpp"


namespace Abytek
{
    void SetupReflectionSession_Base(const TW_Valid<F_ReflectionSession>& Session)
    {
        SetupReflectionSession_Base_Serializable(Session);
        SetupReflectionSession_Base_Math(Session);
    }
}
