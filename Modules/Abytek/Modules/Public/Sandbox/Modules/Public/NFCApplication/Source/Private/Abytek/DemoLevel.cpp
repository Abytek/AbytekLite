#include "Abytek/DemoLevel.hpp"
#include "Abytek/Actor/Actor.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_DemoLevel)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_DemoLevel"));
    }
    
    F_DemoLevel::F_DemoLevel(const F_SerializableObjectInitParams& InitParams) :
        F_Level(InitParams)
    {
    }
    F_DemoLevel::~F_DemoLevel()
    {
    }

    void F_DemoLevel::OnLoadContent()
    {
        TS<F_Actor> Actor;
        if (
            H_Serializable::PopulateObject<F_Actor>(
                Actor,
                ABYTEK_NAME("DemoActor"),
                GetPackageName()
            )
        )
        {
            AddActor(Actor);
        }
    }
}
