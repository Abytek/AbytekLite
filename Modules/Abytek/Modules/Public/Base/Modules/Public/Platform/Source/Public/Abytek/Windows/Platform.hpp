#pragma once

#include "Abytek/Platform.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API F_WindowsPlatform : public A_Platform
    {
    public:
        F_WindowsPlatform();
        virtual ~F_WindowsPlatform() override;
    };
}
#endif