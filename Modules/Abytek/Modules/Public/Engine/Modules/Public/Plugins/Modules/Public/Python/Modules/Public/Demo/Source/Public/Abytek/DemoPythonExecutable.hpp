#pragma once

#include "Abytek/Executable.hpp"


namespace Abytek
{
    class F_DemoPythonExecutable : public F_Executable
    {
    public:
        F_DemoPythonExecutable(const F_ExecutableInput& Input);
        ~F_DemoPythonExecutable() override = default;

    protected:
        virtual void OnStartup() override;
    };
}