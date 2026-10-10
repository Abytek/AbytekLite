#include "Abytek/StandardMaterialSampleLevel.hpp"
#include "Abytek/Assets/StandardMaterial.hpp"
#include "Abytek/World/WorldContextHelper.hpp"


namespace Abytek
{ 
    ABYTEK_REFLECT(F_StandardMaterialSampleLevel)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_StandardMaterialSampleLevel"));
    }
    
    F_StandardMaterialSampleLevel::F_StandardMaterialSampleLevel(const F_SerializableObjectInitParams& InitParams) :
        F_Level(InitParams)
    {
    }
    F_StandardMaterialSampleLevel::~F_StandardMaterialSampleLevel()
    {
    }

    void F_StandardMaterialSampleLevel::OnLoadContent()
    {
        TS<F_StandardMaterial> StandardMaterial;
        if (
            H_WorldContext::PopulateObject<F_StandardMaterial>(
                ABYTEK_WTHIS(),
                StandardMaterial,
                ABYTEK_NAME("DemoStandardMaterial"),
                ABYTEK_NAME("@Abytek.Sandbox.NFCSamples.StandardMaterial::Assets:/.IgnoreSVC/DemoStandardMaterial")
            )    
        )
        {
            StandardMaterial->GetPackage()->Save();
        }
    }
}
