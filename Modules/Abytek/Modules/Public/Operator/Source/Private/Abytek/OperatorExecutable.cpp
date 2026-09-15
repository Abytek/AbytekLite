#include "Abytek/OperatorExecutable.hpp"


namespace Abytek
{
    F_OperatorExecutable::F_OperatorExecutable(const F_ExecutableInput& Input) :
        F_Executable(Input)
    {
    }
    
    void F_OperatorExecutable::OnStartup()
    {
        F_Executable::OnStartup();

        using namespace WithCrtAllocator;

        F_EngineConfig EngineConfig;
        EngineConfig.Commands_Init.push_back(
            [](const TW_Valid<F_EngineRoot>& EngineRoot)
            {
                auto ApplicationManager = F_ApplicationManager::GetInstance();

                auto OperatorApplication = ApplicationManager->CreateApplication<F_OperatorApplication>();
            }
        );
        
        auto EngineRoot = TU<F_EngineRoot>()(
            EngineConfig
        );
        EngineRoot->Start();
    }
}


ABYTEK_DEFINE_EXECUTABLE(Abytek::F_OperatorExecutable);