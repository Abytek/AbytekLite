#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"
#include "Abytek/IHIButton.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API A_IHIButtonCoder : public A_RAObject
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_IHIButtonCoder);
        
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_IHIButtonCoder);
        virtual void Build();
        virtual void Release() override;

    public:
        virtual F_FeedbackStatus Encode(E_IHIPredefinedButton PredefinedButton, F_IHIButton& OutButton) = 0;
        virtual F_FeedbackStatus Decode(E_IHIPredefinedButton& OutPredefinedButton, const F_IHIButton& Button) = 0;
    };
}
