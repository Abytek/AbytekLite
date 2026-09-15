#include "Abytek/ProgramUtilities/ProgramUnit.hpp"
#include "Abytek/ProgramUtilities/ProgramUnitContainer.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_ProgramUnit)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ProgramUnit"));
    }
    
    F_Name F_ProgramUnit::GetFullName() const noexcept
    {
        return ToText(_Container->GetFullName()->c_str()) + ABYTEK_TEXT("::") + _Name->c_str();
    }

    F_ProgramUnit::F_ProgramUnit(const F_ProgramUnitBuildParams& BuildParams) :
        _Container(BuildParams.Container),
        _Name(BuildParams.Name),
        _IsPersistent(BuildParams.IsPersistent)
    {
        _Console = _Container->GetConsole();
        _ReflectionSession = F_ReflectionContext::GetGlobal()->CreateSession({});
    }
    F_ProgramUnit::~F_ProgramUnit()
    {
    }

    void F_ProgramUnit::OnReflect()
    {
    }
    void F_ProgramUnit::OnPostReflect()
    {
    }
    void F_ProgramUnit::OnConfig()
    {
    }
    void F_ProgramUnit::OnPostConfig()
    {
    }

    void F_ProgramUnit::OnInit()
    {
    }
    void F_ProgramUnit::OnPostInit()
    {
    }
    void F_ProgramUnit::OnPreRelease()
    {
    }
    void F_ProgramUnit::OnRelease()
    {
    }

    TF_Vector<TW<F_ReflectionSession>> F_ProgramUnit::GatherReflectionSessions() const
    {
        TF_Vector<TW<F_ReflectionSession>> Result;
        Result.push_back(_ReflectionSession);
        return ABYTEK_MOVE(Result);
    }
}
