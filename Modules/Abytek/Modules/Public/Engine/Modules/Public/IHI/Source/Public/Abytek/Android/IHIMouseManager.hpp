#pragma once

#include "Abytek/IHIMouseManager.hpp"
#include "Abytek/LLEvents/Android/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API F_AndroidIHIMouseManager : public A_IHIMouseManager
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_AndroidIHIMouseManager);
        virtual void Build() override;
        virtual void Release() override;
        
    protected:
        virtual void OnUpdate() override;
        virtual void OnPostUpdate() override;
    };
}
#endif