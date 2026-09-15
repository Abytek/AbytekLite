#include "Abytek/IHIButtonCoder.hpp"
#include "Abytek/IHISubsystem.hpp"
#include "Abytek/Windows/IHIButtonCoder.hpp"
#include "Abytek/Android/IHIButtonCoder.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_IHIButtonCoder);

    ABYTEK_RA_BEGIN_OBJECT(A_IHIButtonCoder)
        auto& IHISubsystem = F_IHISubsystem::GetInstance();
        switch (IHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_PLATFORM_WINDOWS
        case E_IHIAPI::WINDOWS:
            return TU<F_WindowsIHIButtonCoder>()();
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
        case E_IHIAPI::ANDROID_:
            return TU<F_AndroidIHIButtonCoder>()();
#endif
        }
    ABYTEK_RA_END_OBJECT()
    void A_IHIButtonCoder::Build()
    {
        A_RAObject::BuildMinimal();
        
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    void A_IHIButtonCoder::Release()
    {
        A_RAObject::Release();
    }
}
