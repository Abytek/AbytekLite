#pragma once

#include "Abytek/Window.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_ENGINE_WINDOW_API F_AndroidWindow : public A_Window
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_AndroidWindow);
        virtual void Build(const F_WindowBuildParams& Params) override;
        virtual void Release() override;

    private:
        void _CreateWindow();
        void _DestroyWindow();
        
    public:
        virtual void FinalizeNextState() override;
    };
}
#endif