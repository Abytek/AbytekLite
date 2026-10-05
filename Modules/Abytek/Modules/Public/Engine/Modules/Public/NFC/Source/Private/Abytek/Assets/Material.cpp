#include "Abytek/Assets/Material.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_MaterialProperty)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_MaterialProperty"));    
    }
    
    A_MaterialProperty::A_MaterialProperty(const F_SerializableObjectInitParams& InitParam) :
        A_WorldContext(InitParam)
    {
    }
    A_MaterialProperty::~A_MaterialProperty()
    {
    }

    void A_MaterialProperty::OnLoad()
    {
    }
    void A_MaterialProperty::OnUnload()
    {
    }
    
    ABYTEK_REFLECT(F_MaterialPropertyTexture)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MaterialPropertyTexture"));    
    }
    
    F_MaterialPropertyTexture::F_MaterialPropertyTexture(const F_SerializableObjectInitParams& InitParam) :
        A_MaterialProperty(InitParam)
    {
    }
    F_MaterialPropertyTexture::~F_MaterialPropertyTexture()
    {
    }
    
    ABYTEK_REFLECT(F_MaterialRenderPack)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MaterialRenderPack"));
    }

    F_MaterialRenderPack::F_MaterialRenderPack(const F_SerializableObjectInitParams& InitParams) :
        F_RenderPack(InitParams)
    {
    }
    F_MaterialRenderPack::~F_MaterialRenderPack()
    {
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void F_MaterialRenderPack::PrepareTemplates(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap
    )
    {
    }
#endif
    
    ABYTEK_REFLECT(F_Material)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Material"));    
    }
    
    F_Material::F_Material(const F_SerializableObjectInitParams& InitParam) :
        A_MaterialInterface(InitParam)
    {
        _RenderPack = CreateSerializableSubobjectDelayLoading<F_MaterialRenderPack>(
            ABYTEK_NAME("RenderPack")
        );
        _RenderPack->_Material = ABYTEK_WTHIS();
    }
    F_Material::~F_Material()
    {
    }

    void F_Material::OnLoad()
    {
    }
    void F_Material::OnUnload()
    {
    }

    TS<F_Material> F_Material::GetMaterial() const
    {
        return ABYTEK_STHIS_MUTABLE();
    }
    TS<F_MaterialInstance> F_Material::GetMaterialInstance() const
    {
        return _MainInstance;
    }
}
