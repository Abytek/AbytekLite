#include "Abytek/ANodeExecutable.hpp"


namespace Abytek
{
    F_ANodeExecutable::F_ANodeExecutable(const F_ExecutableInput& Input) :
        F_Executable(Input)
    {
    }
    
    void F_ANodeExecutable::OnStartup()
    {
        F_Executable::OnStartup();

        using namespace WithCrtAllocator;

        F_EngineConfig EngineConfig;
        EngineConfig.Commands_Init.push_back(
            [](const TW_Valid<F_EngineRoot>& EngineRoot)
            {
                auto ApplicationManager = F_ApplicationManager::GetInstance();

                auto ANodeApplication = ApplicationManager->CreateApplication<F_ANodeApplication>();
            }
        );
        
        auto EngineRoot = TU<F_EngineRoot>()(
            EngineConfig
        );
        EngineRoot->Start();
    }
}


ABYTEK_DEFINE_EXECUTABLE(Abytek::F_ANodeExecutable);