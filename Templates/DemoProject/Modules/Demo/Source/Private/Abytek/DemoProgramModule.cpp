#include "Abytek/DemoProgramModule.hpp"


namespace Abytek
{
    F_DemoProgramModule::F_DemoProgramModule(const TW_Valid<F_ProgramScope>& Scope) :
        F_ApplicationScopeProgramModule(Scope, ABYTEK_MODULE_NAME)
    {
    }
    F_DemoProgramModule::~F_DemoProgramModule()
    {
    }

    void F_DemoProgramModule::BindLinkedReflect(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::BindLinkedReflect(ReflectedContext);
        
        ReflectedContext.RegisterTypeLinkedReflect<F_DemoApplication>();
        ReflectedContext.RegisterTypeLinkedReflect<F_DemoFrameProcessor>();
    }
    void F_DemoProgramModule::ReflectInlineTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::ReflectInlineTypes(ReflectedContext);
    }
    void F_DemoProgramModule::PostReflectTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::PostReflectTypes(ReflectedContext);
    }
}