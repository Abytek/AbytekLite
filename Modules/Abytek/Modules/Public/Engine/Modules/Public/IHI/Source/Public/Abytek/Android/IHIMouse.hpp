#pragma once

#include "Abytek/IHIMouse.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API F_AndroidIHIMouse : public A_IHIMouse
    {
    public:
        friend class F_AndroidIHIMouseManager;
      
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_AndroidIHIMouse);
        virtual void Build() override;
        virtual void Release() override;
        
    public:
        virtual void FinalizeNextState() override;
    };
}
#endif