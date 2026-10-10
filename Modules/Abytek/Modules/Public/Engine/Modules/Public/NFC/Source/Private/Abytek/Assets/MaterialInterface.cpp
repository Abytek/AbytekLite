#include "Abytek/Assets/MaterialInterface.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_MaterialInterface)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_MaterialInterface"));  
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_OverridePropertyInstanceList);
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
        SetupRenderable();
    }
    void A_MaterialInterface::OnUnload()
    {
        CleanUpRenderable();
    }

    void A_MaterialInterface::OnCreateRenderState()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                CastedRenderProxy = GetRenderProxy().FastCast<A_MaterialInterfaceRenderProxy>(),
                CachedName = GetName()
            ]
            {
                CastedRenderProxy->_Name = CachedName;
            }
        );
    }
    void A_MaterialInterface::OnDestroyRenderState()
    {
    }
}
