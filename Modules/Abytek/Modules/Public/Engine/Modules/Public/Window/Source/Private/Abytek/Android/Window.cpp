#include "Abytek/Android/Window.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/LLEvents/Android/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    void F_AndroidWindow::Build(const F_WindowBuildParams& Params)
    {
        A_Window::Build(Params);
    }
    void F_AndroidWindow::Release()
    {
        A_Window::Release();
    }

    void F_AndroidWindow::FinalizeNextState()
    {
        A_Window::FinalizeNextState();
    }
}
#endif