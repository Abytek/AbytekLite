#include "Abytek/WindowModule.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/WindowCommon.hpp"
#include "Abytek/WindowManager.hpp"
#include "Abytek/CursorManager.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_WindowModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_WindowModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_WindowModule);
    
    F_WindowModule::F_WindowModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
    }
    F_WindowModule::~F_WindowModule()
    {
    }

    void F_WindowModule::OnReflect()
    {
        RegisterStaticType<F_WindowSubsystem>();
        RegisterStaticType<E_WindowSizeMode>();
    }

    void F_WindowModule::OnInit()
    {
        A_WindowManager::GlobalInit();
        A_CursorManager::GlobalInit();
    }
    void F_WindowModule::OnRelease()
    {
        A_CursorManager::GlobalRelease();
        A_WindowManager::GlobalRelease();
    }
}
