#pragma once

#include "Abytek/IHIKeyboard.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API F_AndroidIHIKeyboard : public A_IHIKeyboard
    {
    public:
        friend class F_AndroidIHIKeyboardManager;
      
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_AndroidIHIKeyboard);
        virtual void Build() override;
        virtual void Release() override;
        
    public:
        virtual void FinalizeNextState() override;
    };
}
#endif