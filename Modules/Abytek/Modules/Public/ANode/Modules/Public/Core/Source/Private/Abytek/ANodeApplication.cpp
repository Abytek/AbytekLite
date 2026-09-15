#include "Abytek/ANodeApplication.hpp"
#include "Abytek/ApplicationProgramScope.hpp"
#include "Abytek/ANodeCoreProgramModule.hpp"


namespace Abytek
{
    F_ApplicationConfig SetupANodeApplicationConfig(F_ANodeApplication& ANodeApplication, const F_ApplicationConfig& Config)
    {
        F_ApplicationConfig Result = Config;
        Result.Commands_Init.push_back(
            [&ANodeApplication](const WithCrtAllocator::TW_Valid<F_Application>&)
            {
                auto ProgramScope = ANodeApplication.GetProgramScope();
                ProgramScope->AddModule<F_ANodeCoreProgramModule>();
                
            }
        );
        return boost::move(Result);
    }
    
    F_ANodeApplication::F_ANodeApplication(const F_ApplicationConfig& Config) :
        F_Application(
            SetupANodeApplicationConfig(*this, Config)
        )
    {
    }
    F_ANodeApplication::~F_ANodeApplication()
    {
    }

    B8 F_ANodeApplication::Init()
    {
        F_SurfaceDesc SurfaceDesc;
        SurfaceDesc.Title = ABYTEK_TEXT("Abytek.ANode");
        A_SurfaceManager::GetInstance()->CreateSurface(SurfaceDesc);
        return F_Application::Init();
    }
    B8 F_ANodeApplication::PostInit()
    {
        return F_Application::PostInit();
    }
    B8 F_ANodeApplication::PreRelease()
    {
        return F_Application::PreRelease();
    }
    B8 F_ANodeApplication::Release()
    {
        return F_Application::Release();
    }
}
