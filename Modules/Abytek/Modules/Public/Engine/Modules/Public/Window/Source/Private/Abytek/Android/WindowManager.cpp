#include "Abytek/Android/WindowManager.hpp"
#include "Abytek/Android/Window.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/LLEvents/Android/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    void F_AndroidWindowManager::Build()
    {
        A_WindowManager::Build();

        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_AndroidLLEvents>();
    }
    void F_AndroidWindowManager::Release()
    {
        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_AndroidLLEvents>();

        A_WindowManager::Release();
    }

    void F_AndroidWindowManager::OnUpdate()
    {
        A_WindowManager::OnUpdate();
    }
    void F_AndroidWindowManager::OnPostUpdate()
    {
        A_WindowManager::OnPostUpdate();
    }
}
#endif