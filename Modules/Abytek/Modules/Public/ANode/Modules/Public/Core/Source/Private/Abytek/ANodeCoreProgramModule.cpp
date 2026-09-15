#include "Abytek/ANodeCoreProgramModule.hpp"


namespace Abytek
{
    F_ANodeCoreProgramModule::F_ANodeCoreProgramModule(const WithCrtAllocator::TW_Valid<F_ProgramScope>& Scope, const WithCrtAllocator::F_Text& Name) :
        F_ApplicationScopeProgramModule(Scope, Name)
    {
    }
    F_ANodeCoreProgramModule::~F_ANodeCoreProgramModule()
    {
    }

    void F_ANodeCoreProgramModule::BindLinkedReflect(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::BindLinkedReflect(ReflectedContext);
    }
    void F_ANodeCoreProgramModule::ReflectInlineTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::ReflectInlineTypes(ReflectedContext);
    }
    void F_ANodeCoreProgramModule::PostReflectTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::PostReflectTypes(ReflectedContext);
    }
}