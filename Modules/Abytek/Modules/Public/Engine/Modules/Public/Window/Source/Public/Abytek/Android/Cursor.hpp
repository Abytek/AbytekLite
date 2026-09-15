#pragma once

#include "Abytek/Cursor.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_ENGINE_WINDOW_API F_AndroidCursor : public A_Cursor
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_AndroidCursor);
        virtual void Build() override;
        virtual void Release() override;
        
    public:
        virtual void FinalizeNextState() override;
    };
}
#endif