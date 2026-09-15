#include "Abytek/Development/Cook/CookUtilities.hpp"
#include "Abytek/Development/Cook/CookGraph.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    B8 H_CookUtilities::HasFunction(const F_Name& Name)
    {
        return F_CookGraph::GetInstance()->HasFunction(Name);
    }
    TW_Valid<F_ExecutionGraphFunction> H_CookUtilities::GetFunction(const F_Name& Name)
    {
        return F_CookGraph::GetInstance()->GetFunction(Name);
    }
    TW<F_ExecutionGraphFunction> H_CookUtilities::FindFunction(const F_Name& Name)
    {
        return F_CookGraph::GetInstance()->FindFunction(Name);
    }
    TW_Valid<F_ExecutionGraphFunction> H_CookUtilities::RegisterFunction(TF_Function<void()>&& Functor, const F_Name& Name)
    {
        return F_CookGraph::GetInstance()->RegisterFunction(ABYTEK_MOVE(Functor), Name);
    }
    void H_CookUtilities::UnregisterFunction(const TW_Valid<F_ExecutionGraphFunction>& Function)
    {
        return F_CookGraph::GetInstance()->UnregisterFunction(Function);
    }
    void H_CookUtilities::UnregisterFunction(const F_Name& Name)
    {
        return F_CookGraph::GetInstance()->UnregisterFunction(Name);
    }
}
#endif