#include "Abytek/Assets/MaterialInstance.hpp"
#include "Abytek/Assets/Material.hpp"
#include "Abytek/Assets/Render/MaterialInstanceRenderProxy.hpp"
#include "Abytek/Assets/Render/MaterialRenderProxy.hpp"


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
        A_MaterialInterface::OnLoad();
    }
    void F_MaterialInstance::OnUnload()
    {
        A_MaterialInterface::OnUnload();
    }

    TS<A_Material> F_MaterialInstance::GetMaterial() const
    {
        return _Material;
    }

    B8 F_MaterialInstance::IsRenderable() const
    {
        if (!A_MaterialInterface::IsRenderable())
        {
            return false;
        }
        if (!_Material)
        {
            return false;
        }
        return _Material->IsRenderable();
    }

    TS<A_RenderProxy> F_MaterialInstance::CreateRenderProxy()
    {
        return TS<F_MaterialInstanceRenderProxy>()(ABYTEK_WTHIS());
    }

    void F_MaterialInstance::OnCreateRenderState()
    {
        A_MaterialInterface::OnCreateRenderState();
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                CastedRenderProxy = GetRenderProxy().FastCast<F_MaterialInstanceRenderProxy>(),
                CachedMaterialRenderProxy = _Material->GetRenderPack().FastCast<F_MaterialRenderProxy>()
            ]
            {
                CastedRenderProxy->_MaterialRenderProxy = CachedMaterialRenderProxy;
            }
        );
    }
    void F_MaterialInstance::OnDestroyRenderState()
    {
        A_MaterialInterface::OnDestroyRenderState();
    }
}
