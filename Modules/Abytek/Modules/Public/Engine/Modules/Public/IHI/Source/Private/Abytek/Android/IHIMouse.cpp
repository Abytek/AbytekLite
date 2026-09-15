#include "Abytek/Android/IHIMouse.hpp"
#include "Abytek/IHISubsystem.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    void F_AndroidIHIMouse::Build()
    {
        A_IHIMouse::Build();
    }
    void F_AndroidIHIMouse::Release()
    {
        A_IHIMouse::Release();
    }

    void F_AndroidIHIMouse::FinalizeNextState()
    {
        A_IHIMouse::FinalizeNextState();
    }
}
#endif