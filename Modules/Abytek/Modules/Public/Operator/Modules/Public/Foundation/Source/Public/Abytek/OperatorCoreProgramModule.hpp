#pragma once

#include "Abytek/Operator.Core.prerequisites.hpp"
#include "Abytek/ApplicationScopeProgramModule.hpp"


namespace Abytek
{
    class ABYTEK_OPERATOR_CORE_API F_OperatorCoreProgramModule : public F_ApplicationScopeProgramModule
    {
    public:
        F_OperatorCoreProgramModule(const WithCrtAllocator::TW_Valid<F_ProgramScope>& Scope, const WithCrtAllocator::F_Text& Name = ABYTEK_TEXT("Core"));
        ~F_OperatorCoreProgramModule();

    protected:
        virtual void BindLinkedReflect(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
        virtual void ReflectInlineTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
        virtual void PostReflectTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
    };
}
