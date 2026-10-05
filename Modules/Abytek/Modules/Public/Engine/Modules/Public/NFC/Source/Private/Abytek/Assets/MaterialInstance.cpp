#include "Abytek/Assets/MaterialInstance.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_MaterialInstance)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MaterialInstance"));    
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Material);
    }
    
    F_MaterialInstance::F_MaterialInstance(const F_SerializableObjectInitParams& InitParam) :
        A_MaterialInterface(InitParam)
    {
    }
    F_MaterialInstance::~F_MaterialInstance()
    {
    }

    void F_MaterialInstance::OnLoad()
    {
    }
    void F_MaterialInstance::OnUnload()
    {
    }

    TS<F_Material> F_MaterialInstance::GetMaterial() const
    {
        return _Material;
    }
    TS<F_MaterialInstance> F_MaterialInstance::GetMaterialInstance() const
    {
        return ABYTEK_STHIS_MUTABLE();
    }
}
