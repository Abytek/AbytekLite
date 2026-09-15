#pragma once

#include "Abytek/IHIKeyboard.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API F_WindowsIHIKeyboard : public A_IHIKeyboard
    {
    public:
        friend class F_WindowsIHIKeyboardManager;
      
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_WindowsIHIKeyboard);
        virtual void Build() override;
        virtual void Release() override;
        
    public:
        virtual void FinalizeNextState() override;
    };
}
#endif