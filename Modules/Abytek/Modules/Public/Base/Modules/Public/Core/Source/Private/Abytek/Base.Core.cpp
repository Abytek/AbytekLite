#include "Abytek/Log.hpp"
#include "Abytek/Assert.hpp"

namespace Abytek
{
    namespace Internal
    {
        thread_local U32 LogHiddenCounter = 0;
    }
    
    U32 GetLogHiddenCounter()
    {
        return Internal::LogHiddenCounter;
    }
    U32 IncreaseLogHiddenCounter(U32 Num)
    {
        U32 Result = Internal::LogHiddenCounter;
        Internal::LogHiddenCounter += Num;
        return Result;
    }
    U32 DecreaseLogHiddenCounter(U32 Num)
    {
        U32 Result = Internal::LogHiddenCounter;
        Internal::LogHiddenCounter -= Num;
        return Result;
    }
}