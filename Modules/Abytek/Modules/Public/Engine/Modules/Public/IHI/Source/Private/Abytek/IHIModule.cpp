#include "Abytek/IHIModule.hpp"
#include "Abytek/IHISubsystem.hpp"
#include "Abytek/IHIButton.hpp"
#include "Abytek/IHIMouseManager.hpp"
#include "Abytek/IHIKeyboardManager.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_IHIModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_IHIModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_IHIModule);
    
    F_IHIModule::F_IHIModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
    }
    F_IHIModule::~F_IHIModule()
    {
    }

    void F_IHIModule::OnReflect()
    {
        RegisterStaticType<F_IHISubsystem>();
        RegisterStaticType<E_IHIPredefinedButton>();
        RegisterStaticType<E_IHIButtonDeviceType>();
        RegisterStaticType<F_IHIButton>();
    }

    void F_IHIModule::OnInit()
    {
        A_IHIMouseManager::GlobalInit();
        A_IHIKeyboardManager::GlobalInit();
    }
    void F_IHIModule::OnRelease()
    {
        A_IHIKeyboardManager::GlobalRelease();
        A_IHIMouseManager::GlobalRelease();
    }
}
