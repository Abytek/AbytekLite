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
    
    ABYTEK_REFLECT(A_MaterialRenderPack)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MaterialRenderPack"));
    }

    A_MaterialRenderPack::A_MaterialRenderPack(const F_SerializableObjectInitParams& InitParams) :
        F_RenderPack(InitParams)
    {
    }
    A_MaterialRenderPack::~A_MaterialRenderPack()
    {
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void A_MaterialRenderPack::PrepareTemplates(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap
    )
    {
    }
#endif
    
    ABYTEK_REFLECT(A_Material)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Material"));    
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(RenderPackType);
    }
    
    A_Material::A_Material(const F_SerializableObjectInitParams& InitParam) :
        A_MaterialInterface(InitParam)
    {
    }
    A_Material::~A_Material()
    {
    }

    void A_Material::OnPostConstruct()
    {
        A_MaterialInterface::OnPostConstruct();
        
        _RenderPack = CreateSerializableSubobjectDelayLoading<A_MaterialRenderPack>(
            ABYTEK_NAME("RenderPack"),
            RenderPackType
        );
        _RenderPack->_Material = ABYTEK_WTHIS();
    }
    void A_Material::OnPreDestruct()
    {
        A_MaterialInterface::OnPreDestruct();
    }

    void A_Material::OnLoad()
    {
    }
    void A_Material::OnUnload()
    {
    }

    TS<A_Material> A_Material::GetMaterial() const
    {
        return ABYTEK_STHIS_MUTABLE();
    }
    TS<F_MaterialInstance> A_Material::GetMaterialInstance() const
    {
        return _MainInstance;
    }
}
