#include "Abytek/DemoPackageProgramModule.hpp"


namespace Abytek
{
    F_DemoPackageProgramModule::F_DemoPackageProgramModule(const TW_Valid<F_ProgramScope>& Scope) :
        F_ApplicationScopeProgramModule(Scope, ABYTEK_MODULE_NAME)
    {
    }
    F_DemoPackageProgramModule::~F_DemoPackageProgramModule()
    {
    }

    void F_DemoPackageProgramModule::BindLinkedReflect(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::BindLinkedReflect(ReflectedContext);
        
        ReflectedContext.RegisterTypeLinkedReflect<F_DemoPackageApplication>();
        ReflectedContext.RegisterTypeLinkedReflect<F_DemoPackageFrameProcessor>();
    }
    void F_DemoPackageProgramModule::ReflectInlineTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::ReflectInlineTypes(ReflectedContext);
    }
    void F_DemoPackageProgramModule::PostReflectTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext)
    {
        F_ApplicationScopeProgramModule::PostReflectTypes(ReflectedContext);
    }
}