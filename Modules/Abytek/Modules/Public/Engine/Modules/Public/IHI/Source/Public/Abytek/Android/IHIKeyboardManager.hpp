#pragma once

#include "Abytek/IHIKeyboardManager.hpp"
#include "Abytek/LLEvents/Android/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API F_AndroidIHIKeyboardManager : public A_IHIKeyboardManager
    { 
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_AndroidIHIKeyboardManager);
        virtual void Build() override;
        virtual void Release() override;
        
    protected:
        virtual void OnUpdate() override;
        virtual void OnPostUpdate() override;
    };
}
#endif