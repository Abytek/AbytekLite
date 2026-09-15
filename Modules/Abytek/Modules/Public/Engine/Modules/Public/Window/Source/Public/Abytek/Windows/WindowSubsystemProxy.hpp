#pragma once

#include "Abytek/WindowSubsystemProxy.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    class ABYTEK_ENGINE_WINDOW_API F_WindowsWindowSubsystemProxy : public A_WindowSubsystemProxy
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_WindowsWindowSubsystemProxy);
        virtual void Build() override;
        virtual void Release() override;
        
    public:
        E_WindowAPI GetAPI() override;
    };
}
#endif