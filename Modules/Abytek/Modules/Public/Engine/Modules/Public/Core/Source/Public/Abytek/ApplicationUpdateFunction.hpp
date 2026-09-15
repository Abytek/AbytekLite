#pragma once

#include "Abytek/UpdateBase/UpdateUtilities.hpp"
#include "Abytek/ApplicationCore.hpp"


namespace Abytek
{
    struct ABYTEK_ENGINE_CORE_API H_ApplicationUpdateFunction
    {
        static TW_Valid<F_ExecutionGraphFunction> Register(TF_Function<void()>&& Functor, const F_Name& Name = {}, E_ApplicationState States = E_ApplicationState::ALL);
        static TW_Valid<F_ExecutionGraphFunction> Register(TF_Function<void()>&& Functor, E_ApplicationState States)
        {
            return Register(ABYTEK_MOVE(Functor), {}, States);
        }
        static TW_Valid<F_ExecutionGraphFunction> Register(const F_Name& Name = {}, E_ApplicationState States = E_ApplicationState::ALL)
        {
            return Register([]{}, ABYTEK_MOVE(Name), States);
        }
        static TW_Valid<F_ExecutionGraphFunction> Register(E_ApplicationState States)
        {
            return Register([]{}, {}, States);
        }
    };
}