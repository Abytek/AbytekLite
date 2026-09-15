#include "Abytek/Android/CursorManager.hpp"
#include "Abytek/Android/Cursor.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/LLEvents/Android/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    void F_AndroidCursorManager::Build()
    {
        A_CursorManager::Build();

        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_AndroidLLEvents>();
    }
    void F_AndroidCursorManager::Release()
    {
        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_AndroidLLEvents>();

        A_CursorManager::Release();
    }

    void F_AndroidCursorManager::OnUpdate()
    {
        A_CursorManager::OnUpdate();
    }
    void F_AndroidCursorManager::OnPostUpdate()
    {
        A_CursorManager::OnPostUpdate();
    }
}
#endif