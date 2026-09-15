#pragma once

#include "Abytek/Templates.DemoPackage.prerequisites.pch.hpp"
#include "Abytek/ApplicationScopeProgramModule.hpp"


namespace Abytek
{
    class F_DemoPackageProgramModule final : public F_ApplicationScopeProgramModule
    {
    public:
        F_DemoPackageProgramModule(const TW_Valid<F_ProgramScope>& Scope);
        ~F_DemoPackageProgramModule();

    protected:
        virtual void BindLinkedReflect(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
        virtual void ReflectInlineTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
        virtual void PostReflectTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
    };
}
