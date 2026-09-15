#pragma once

#include "Abytek/IHISubsystemProxy.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API F_AndroidIHISubsystemProxy : public A_IHISubsystemProxy
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_AndroidIHISubsystemProxy);
        virtual void Build() override;
        virtual void Release() override;

    protected:
        virtual E_IHIAPI GetAPI() override;

    public:
        static TU<A_IHISubsystemProxy> TryCreate();
    };
}
#endif