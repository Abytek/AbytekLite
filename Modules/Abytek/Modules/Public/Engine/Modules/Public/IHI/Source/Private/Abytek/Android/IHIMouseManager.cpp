#include "Abytek/Android/IHIMouseManager.hpp"
#include "Abytek/Android/IHIMouse.hpp"
#include "Abytek/IHISubsystem.hpp"
#include "Abytek/LLEvents/Android/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    void F_AndroidIHIMouseManager::Build()
    {
        A_IHIMouseManager::Build();

        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_AndroidLLEvents>();
    }
    void F_AndroidIHIMouseManager::Release()
    {
        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_AndroidLLEvents>();

        A_IHIMouseManager::Release();
    }

    void F_AndroidIHIMouseManager::OnUpdate()
    {
        A_IHIMouseManager::OnUpdate();
    }
    void F_AndroidIHIMouseManager::OnPostUpdate()
    {
        A_IHIMouseManager::OnPostUpdate();
    }
}
#endif