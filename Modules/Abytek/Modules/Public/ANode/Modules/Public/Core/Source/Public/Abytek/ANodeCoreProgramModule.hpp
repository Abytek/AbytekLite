#pragma once

#include "Abytek/ANode.Core.prerequisites.hpp"
#include "Abytek/ApplicationScopeProgramModule.hpp"


namespace Abytek
{
    class ABYTEK_ANODE_CORE_API F_ANodeCoreProgramModule : public F_ApplicationScopeProgramModule
    {
    public:
        F_ANodeCoreProgramModule(const WithCrtAllocator::TW_Valid<F_ProgramScope>& Scope, const WithCrtAllocator::F_Text& Name = ABYTEK_TEXT("Core"));
        ~F_ANodeCoreProgramModule();

    protected:
        virtual void BindLinkedReflect(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
        virtual void ReflectInlineTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
        virtual void PostReflectTypes(WithCrtAllocator::F_ReflectedContext& ReflectedContext) override;
    };
}
