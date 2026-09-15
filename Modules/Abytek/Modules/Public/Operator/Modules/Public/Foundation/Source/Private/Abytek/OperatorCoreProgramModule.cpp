#include "Abytek/OperatorCoreProgramModule.hpp"


namespace Abytek
{
    F_OperatorCoreProgramModule::F_OperatorCoreProgramModule(const WithCrtAllocator::TW_Valid<F_ProgramScope>& Scope, const WithCrtAllocator::F_Text& Name) :
        F_ApplicationScopeProgramModule(Scope, Name)
    {
    }
    F_OperatorCoreProgramModule::~F_OperatorCoreProgramModule()
    {
    }

    void F_OperatorCoreProgramModule::BindLinkedReflect(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::BindLinkedReflect(ReflectedContext);
    }
    void F_OperatorCoreProgramModule::ReflectInlineTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::ReflectInlineTypes(ReflectedContext);
    }
    void F_OperatorCoreProgramModule::PostReflectTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::PostReflectTypes(ReflectedContext);
    }
}