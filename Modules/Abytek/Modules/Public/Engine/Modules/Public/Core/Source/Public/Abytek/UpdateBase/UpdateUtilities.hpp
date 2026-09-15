#pragma once

#include "Abytek/ExecutionGraph.hpp"


namespace Abytek
{
    struct ABYTEK_ENGINE_CORE_API H_UpdateUtilities
    {
        static B8 HasFunction(const F_Name& Name);
        static TW_Valid<F_ExecutionGraphFunction> GetFunction(const F_Name& Name);
        static TW<F_ExecutionGraphFunction> FindFunction(const F_Name& Name);
        static TW_Valid<F_ExecutionGraphFunction> RegisterFunction(TF_Function<void()>&& Functor, const F_Name& Name = {});
        static TW_Valid<F_ExecutionGraphFunction> RegisterFunction(const F_Name& Name = {})
        {
            return RegisterFunction([]{}, Name);
        }
        static void UnregisterFunction(const TW_Valid<F_ExecutionGraphFunction>& Function);
        static void UnregisterFunction(const F_Name& Name);
    };
}
