#pragma once

#include "Abytek/Templates.Demo.prerequisites.pch.hpp"
#include "Abytek/ApplicationScopeProgramModule.hpp"


namespace Abytek
{
    class F_DemoProgramModule final : public F_ApplicationScopeProgramModule
    {
    public:
        F_DemoProgramModule(const TW_Valid<F_ProgramScope>& Scope);
        ~F_DemoProgramModule();

    protected:
        virtual void BindLinkedReflect(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
        virtual void ReflectInlineTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
        virtual void PostReflectTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
    };
}
