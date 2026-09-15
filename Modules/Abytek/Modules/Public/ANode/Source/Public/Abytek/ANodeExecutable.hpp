#pragma once

#include "Abytek/ANode.prerequisites.hpp"
#include "Abytek/Executable.hpp"


namespace Abytek
{
    class F_ANodeExecutable : public F_Executable
    {
    public:
        F_ANodeExecutable(const F_ExecutableInput& Input);
        ~F_ANodeExecutable() override = default;

    protected:
        virtual void OnStartup() override;
    };
}