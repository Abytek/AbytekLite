#include "Abytek/CanvasSampleLevel.hpp"
#include "Abytek/Actor/Actor.hpp"
#include "Abytek/ActorComponents/CanvasComponent.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_CanvasSampleLevel)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CanvasSampleLevel"));
    }
    
    F_CanvasSampleLevel::F_CanvasSampleLevel(const F_SerializableObjectInitParams& InitParams) :
        F_Level(InitParams)
    {
    }
    F_CanvasSampleLevel::~F_CanvasSampleLevel()
    {
    }

    void F_CanvasSampleLevel::OnLoadContent()
    {
        TS<F_Actor> Actor;
        if (
            H_Serializable::PopulateObject<F_Actor>(
                Actor,
                ABYTEK_NAME("CanvasActor"),
                GetPackageName()
            )    
        )
        {
            TS<F_CanvasComponent> CanvasComponent;
            if (
                H_Serializable::PopulateObject<F_CanvasComponent>(
                    CanvasComponent,
                    ABYTEK_NAME("CanvasComponent"),
                    GetPackageName()
                )    
            )
            {
                Actor->AddOwnedComponent(CanvasComponent);
            }
            AddActor(Actor);
        }
        GetPackage()->Save();
    }
}
