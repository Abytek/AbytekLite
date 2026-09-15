#pragma once

#include "Abytek/LLEvents/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_AndroidLLEvents : public A_LLEvents
    {
    private:

    public:

    public:
        F_AndroidLLEvents();
        ~F_AndroidLLEvents() override;
        
    private:
        void _AndroidProcessMessages();

    public:
        virtual void Start() override;
        virtual void FinalizeFrame() override;
    };
}
#endif