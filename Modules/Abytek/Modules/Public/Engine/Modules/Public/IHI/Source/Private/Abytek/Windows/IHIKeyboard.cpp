#include "Abytek/Windows/IHIKeyboard.hpp"
#include "Abytek/IHISubsystem.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    void F_WindowsIHIKeyboard::Build()
    {
        A_IHIKeyboard::Build();
    }
    void F_WindowsIHIKeyboard::Release()
    {
        A_IHIKeyboard::Release();
    }

    void F_WindowsIHIKeyboard::FinalizeNextState()
    {
        A_IHIKeyboard::FinalizeNextState();
    }
}
#endif