#pragma once

#include "Abytek/Operator.prerequisites.hpp"
#include "Abytek/Executable.hpp"


namespace Abytek
{
    class F_OperatorExecutable : public F_Executable
    {
    public:
        F_OperatorExecutable(const F_ExecutableInput& Input);
        ~F_OperatorExecutable() override = default;

    protected:
        virtual void OnStartup() override;
    };
}