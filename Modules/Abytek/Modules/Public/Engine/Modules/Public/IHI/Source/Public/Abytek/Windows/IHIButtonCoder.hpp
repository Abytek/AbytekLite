#pragma once

#include "Abytek/IHIButtonCoder.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API F_WindowsIHIButtonCoder : public A_IHIButtonCoder
    {
    public:
        friend class F_WindowsIHIButtonCoderManager;
      
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_WindowsIHIButtonCoder);
        void Build() override;
        void Release() override;

    public:
        F_FeedbackStatus Encode(E_IHIPredefinedButton PredefinedButton, F_IHIButton& OutButton) override;
        F_FeedbackStatus Decode(E_IHIPredefinedButton& OutPredefinedButton, const F_IHIButton& Button) override;
    };
}
#endif