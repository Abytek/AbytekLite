#pragma once

#include "Abytek/Cursor.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    class ABYTEK_ENGINE_WINDOW_API F_WindowsCursor : public A_Cursor
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_WindowsCursor);
        virtual void Build() override;
        virtual void Release() override;
        
    public:
        void WindowsMove(const F_Vector2_I32& InValue);
        void WindowsSetVisibility(B8 InValue);
        void WindowsLockToWindow(const TW_Valid<A_Window>& InWindow);
        void WindowsUnlockFromWindow();
        
    public:
        virtual void FinalizeNextState() override;
    };
}
#endif