#pragma once

#include "Abytek/CursorManager.hpp"
#include "Abytek/LLEvents/Android/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_ENGINE_WINDOW_API F_AndroidCursorManager : public A_CursorManager
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_AndroidCursorManager);
        virtual void Build() override;
        virtual void Release() override;
        
    protected:
        virtual void OnUpdate() override;
        virtual void OnPostUpdate() override;
    };
}
#endif