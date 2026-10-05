#include "Abytek/Assets/MaterialInterface.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_MaterialInterface)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_MaterialInterface"));    
    }
    
    A_MaterialInterface::A_MaterialInterface(const F_SerializableObjectInitParams& InitParam) :
        A_WorldContext(InitParam)
    {
    }
    A_MaterialInterface::~A_MaterialInterface()
    {
    }

    void A_MaterialInterface::OnLoad()
    {
    }
    void A_MaterialInterface::OnUnload()
    {
    }
}
