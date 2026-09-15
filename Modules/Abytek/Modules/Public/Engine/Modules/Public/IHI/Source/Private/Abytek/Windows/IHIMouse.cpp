#include "Abytek/Windows/IHIMouse.hpp"
#include "Abytek/IHISubsystem.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    void F_WindowsIHIMouse::Build()
    {
        A_IHIMouse::Build();
    }
    void F_WindowsIHIMouse::Release()
    {
        A_IHIMouse::Release();
    }

    void F_WindowsIHIMouse::FinalizeNextState()
    {
        A_IHIMouse::FinalizeNextState();
    }
}
#endif