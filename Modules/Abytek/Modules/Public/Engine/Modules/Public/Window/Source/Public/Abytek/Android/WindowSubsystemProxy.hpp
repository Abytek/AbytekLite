#pragma once

#include "Abytek/WindowSubsystemProxy.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_ENGINE_WINDOW_API F_AndroidWindowSubsystemProxy : public A_WindowSubsystemProxy
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_AndroidWindowSubsystemProxy);
        virtual void Build() override;
        virtual void Release() override;
        
    public:
        E_WindowAPI GetAPI() override;
    };
}
#endif