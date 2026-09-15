#pragma once

#include "Abytek/IHIMouse.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API F_WindowsIHIMouse : public A_IHIMouse
    {
    public:
        friend class F_WindowsIHIMouseManager;
      
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_WindowsIHIMouse);
        virtual void Build() override;
        virtual void Release() override;
        
    public:
        virtual void FinalizeNextState() override;
    };
}
#endif