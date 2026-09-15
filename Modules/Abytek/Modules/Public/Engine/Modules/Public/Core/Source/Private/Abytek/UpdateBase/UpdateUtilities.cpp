#include "Abytek/UpdateBase/UpdateUtilities.hpp"
#include "Abytek/UpdateBase/UpdateManager.hpp"


namespace Abytek
{
    B8 H_UpdateUtilities::HasFunction(const F_Name& Name)
    {
        return F_UpdateManager::GetInstance()->HasFunction(Name);
    }
    TW_Valid<F_ExecutionGraphFunction> H_UpdateUtilities::GetFunction(const F_Name& Name)
    {
        return F_UpdateManager::GetInstance()->GetFunction(Name);
    }
    TW<F_ExecutionGraphFunction> H_UpdateUtilities::FindFunction(const F_Name& Name)
    {
        return F_UpdateManager::GetInstance()->FindFunction(Name);
    }
    TW_Valid<F_ExecutionGraphFunction> H_UpdateUtilities::RegisterFunction(TF_Function<void()>&& Functor, const F_Name& Name)
    {
        return F_UpdateManager::GetInstance()->RegisterFunction(ABYTEK_MOVE(Functor), Name);
    }
    void H_UpdateUtilities::UnregisterFunction(const TW_Valid<F_ExecutionGraphFunction>& Function)
    {
        return F_UpdateManager::GetInstance()->UnregisterFunction(Function);
    }
    void H_UpdateUtilities::UnregisterFunction(const F_Name& Name)
    {
        return F_UpdateManager::GetInstance()->UnregisterFunction(Name);
    }
}
