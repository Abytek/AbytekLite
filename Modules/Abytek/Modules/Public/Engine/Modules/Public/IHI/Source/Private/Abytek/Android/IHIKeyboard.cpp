#include "Abytek/Android/IHIKeyboard.hpp"
#include "Abytek/IHISubsystem.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    void F_AndroidIHIKeyboard::Build()
    {
        A_IHIKeyboard::Build();
    }
    void F_AndroidIHIKeyboard::Release()
    {
        A_IHIKeyboard::Release();
    }

    void F_AndroidIHIKeyboard::FinalizeNextState()
    {
        A_IHIKeyboard::FinalizeNextState();
    }
}
#endif