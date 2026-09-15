#include "Abytek/Android/Cursor.hpp"
#include "Abytek/Android/Window.hpp"
#include "Abytek/WindowSubsystem.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    void F_AndroidCursor::Build()
    {
        A_Cursor::Build();
    }
    void F_AndroidCursor::Release()
    {
        A_Cursor::Release();
    }

    void F_AndroidCursor::FinalizeNextState()
    {
        A_Cursor::FinalizeNextState();
    }
}
#endif