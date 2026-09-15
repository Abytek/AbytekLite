#include "Abytek/Android/IHIKeyboardManager.hpp"
#include "Abytek/Android/IHIKeyboard.hpp"
#include "Abytek/Android/IHIButtonCoder.hpp"
#include "Abytek/IHISubsystem.hpp"
#include "Abytek/LLEvents/Android/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    void F_AndroidIHIKeyboardManager::Build()
    {
        A_IHIKeyboardManager::Build();

        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_AndroidLLEvents>();
    }
    void F_AndroidIHIKeyboardManager::Release()
    {
        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_AndroidLLEvents>();

        A_IHIKeyboardManager::Release();
    }

    void F_AndroidIHIKeyboardManager::OnUpdate()
    {
        A_IHIKeyboardManager::OnUpdate();
    }
    void F_AndroidIHIKeyboardManager::OnPostUpdate()
    {
        A_IHIKeyboardManager::OnPostUpdate();
    }
}
#endif