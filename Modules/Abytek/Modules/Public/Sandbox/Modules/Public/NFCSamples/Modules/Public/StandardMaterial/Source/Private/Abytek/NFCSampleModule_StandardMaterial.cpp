#include "Abytek/NFCSampleModule_StandardMaterial.hpp"
#include "Abytek/StandardMaterialSampleLevel.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_NFCSampleModule_StandardMaterial)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_NFCSampleModule_StandardMaterial"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_StandardMaterial)
    
    F_NFCSampleModule_StandardMaterial::F_NFCSampleModule_StandardMaterial(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_NFCSampleModule_StandardMaterial::~F_NFCSampleModule_StandardMaterial()
    {
    }

    void F_NFCSampleModule_StandardMaterial::OnReflect()
    {
        RegisterStaticType<F_StandardMaterialSampleLevel>();
    }

    void F_NFCSampleModule_StandardMaterial::OnStartup()
    {
    }
}
