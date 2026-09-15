#include "Abytek/CameraSampleLevel.hpp"
#include "Abytek/SampleSpectatorComponent.hpp"
#include "Abytek/Actor/Actor.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_CameraSampleLevel)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CameraSampleLevel"));
    }
    
    F_CameraSampleLevel::F_CameraSampleLevel(const F_SerializableObjectInitParams& InitParams) :
        F_Level(InitParams)
    {
    }
    F_CameraSampleLevel::~F_CameraSampleLevel()
    {
    }

    void F_CameraSampleLevel::OnLoadContent()
    {
        TS<F_Actor> Actor;
        if (
            H_Serializable::PopulateObject<F_Actor>(
                Actor,
                ABYTEK_NAME("SampleSpectatorActor"),
                GetPackageName()
            )    
        )
        {
            TS<F_SampleSpectatorComponent> SampleSpectatorComponent;
            if (
                H_Serializable::PopulateObject<F_SampleSpectatorComponent>(
                    SampleSpectatorComponent,
                    ABYTEK_NAME("SampleSpectatorComponent"),
                    GetPackageName()
                )    
            )
            {
                Actor->AddOwnedComponent(SampleSpectatorComponent);
            }
            
            AddActor(Actor);
        }
        GetPackage()->Save();
    }
}
