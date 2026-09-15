#include "Abytek/OperatorApplication.hpp"
#include "Abytek/ApplicationProgramScope.hpp"
#include "Abytek/OperatorCoreProgramModule.hpp"


namespace Abytek
{
    F_ApplicationConfig SetupOperatorApplicationConfig(F_OperatorApplication& OperatorApplication, const F_ApplicationConfig& Config)
    {
        F_ApplicationConfig Result = Config;
        Result.Commands_Init.push_back(
            [&OperatorApplication](const WithCrtAllocator::TW_Valid<F_Application>&)
            {
                auto ProgramScope = OperatorApplication.GetProgramScope();
                ProgramScope->AddModule<F_OperatorCoreProgramModule>();
                
            }
        );
        return boost::move(Result);
    }
    
    F_OperatorApplication::F_OperatorApplication(const F_ApplicationConfig& Config) :
        F_Application(
            SetupOperatorApplicationConfig(*this, Config)
        )
    {
    }
    F_OperatorApplication::~F_OperatorApplication()
    {
    }

    B8 F_OperatorApplication::Init()
    {
        F_SurfaceDesc SurfaceDesc;
        SurfaceDesc.Title = ABYTEK_TEXT("Abytek.Operator");
        A_SurfaceManager::GetInstance()->CreateSurface(SurfaceDesc);
        return F_Application::Init();
    }
    B8 F_OperatorApplication::PostInit()
    {
        return F_Application::PostInit();
    }
    B8 F_OperatorApplication::PreRelease()
    {
        return F_Application::PreRelease();
    }
    B8 F_OperatorApplication::Release()
    {
        return F_Application::Release();
    }
}
