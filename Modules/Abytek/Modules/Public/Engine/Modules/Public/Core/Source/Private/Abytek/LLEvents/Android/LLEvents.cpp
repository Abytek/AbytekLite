#include "Abytek/LLEvents/Android/LLEvents.hpp"
#include "Abytek/Profiling.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    F_AndroidLLEvents::F_AndroidLLEvents()
    {
    }
    F_AndroidLLEvents::~F_AndroidLLEvents()
    {
    }

    void F_AndroidLLEvents::_AndroidProcessMessages()
    {
    }

    void F_AndroidLLEvents::Start()
    {
        A_LLEvents::Start();
    }
    void F_AndroidLLEvents::FinalizeFrame()
    {
        _AndroidProcessMessages();
    }
}
#endif