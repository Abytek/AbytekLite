#include "Abytek/Sandbox.EmptyExecutable.prerequisites.pch.hpp"


namespace Abytek
{
    class F_EmptyExecutable : public F_Executable
    {
    public:
        F_EmptyExecutable(const F_ExecutableInput& Input);
        ~F_EmptyExecutable() override = default;

    protected:
        void OnStartup() override;
    };
    
    F_EmptyExecutable::F_EmptyExecutable(const F_ExecutableInput& Input) :
        F_Executable(Input)
    {
    }
    
    void F_EmptyExecutable::OnStartup()
    {
        F_Executable::OnStartup();
    }
}

ABYTEK_DEFINE_EXECUTABLE(Abytek::F_EmptyExecutable);
    