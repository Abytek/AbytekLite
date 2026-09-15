#include "Abytek/ApplicationUpdateFunction.hpp"


namespace Abytek
{
    TW_Valid<F_ExecutionGraphFunction> H_ApplicationUpdateFunction::Register(TF_Function<void()>&& Functor, const F_Name& Name, E_ApplicationState States)
    {
        if (States == E_ApplicationState::ALL)
        {
            return H_UpdateUtilities::RegisterFunction(ABYTEK_MOVE(Functor), Name);
        }
        return H_UpdateUtilities::RegisterFunction(
            [CachedFunctor = ABYTEK_MOVE(Functor), States]() mutable
            {
                if (FlagHasAny(A_ApplicationCore::GetInstance()->GetState(), States))
                {
                    CachedFunctor();
                }
            },
            Name
        );
    }
}
