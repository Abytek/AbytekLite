#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/RenderPack.hpp"
#include "Abytek/Assets/MaterialInterface.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API A_MaterialProperty : public A_WorldContext
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(A_MaterialProperty);
        
    private:
        
    public:
        
    protected:
        A_MaterialProperty(const F_SerializableObjectInitParams& InitParam);
        
    public:
        ~A_MaterialProperty() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
    };
    
    namespace Internal::MaterialProperty
    {
        template<typename __F>
        struct TH_ValueForward 
        {
            using F = __F;
        };
        template<>
        struct TH_ValueForward<Sz>
        {
            using F = TF_UInt<sizeof(Sz)>;
        };
        template<>
        struct TH_ValueForward<PDiff>
        {
            using F = TF_Int<sizeof(PDiff)>;
        };
    }
    
    template<typename __F_Value>
    class ABYTEK_ENGINE_NFC_API TF_MaterialPropertyScalar : public A_MaterialProperty
    {
    public:
        using F_Value = typename Internal::MaterialProperty::TH_ValueForward<__F_Value>::F;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_MaterialProperty)
        ABYTEK_END_REFLECTOR(TF_MaterialPropertyScalar)
        {
            auto ValueType = ReflectionType->ReflectReferenced<F_Value>();
            ABYTEK_REFLECT_COMMAND(
                OnReflectCanonicals,
                [=]()
                {
                    if (auto ValueTypeCanonical = ValueType->GetCanonical())
                    {
                        ReflectionType->SetCanonical(
                            ABYTEK_TEXT("Abytek::TF_MaterialPropertyScalar<")
                            *ValueTypeCanonical
                            + ABYTEK_TEXT(">")
                        );
                    }
                }
            );
        }
        
    private:
        F_Value _Value = F_Value(0);
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetValue() const noexcept
        {
            return _Value;
        }
        
    public:
        TF_MaterialPropertyScalar(const F_SerializableObjectInitParams& InitParam) :
            A_MaterialProperty(InitParam)
        {
        }
        ~TF_MaterialPropertyScalar() override
        {
        }
    };
    
    class ABYTEK_ENGINE_NFC_API F_MaterialPropertyTexture : public A_MaterialProperty
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_MaterialProperty)
        ABYTEK_END_REFLECTOR(F_MaterialPropertyTexture);
        
    private:
        
    public:
        
    public:
        F_MaterialPropertyTexture(const F_SerializableObjectInitParams& InitParam);
        ~F_MaterialPropertyTexture() override;
    };
    
    class ABYTEK_ENGINE_NFC_API F_MaterialRenderPack final : public F_RenderPack
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_RenderPack)
        ABYTEK_END_REFLECTOR(F_MaterialRenderPack);
        
    public:
        friend class F_Material;
     
    private:
        TW<F_Material> _Material;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetMaterial() const noexcept
        {
            return _Material;
        }
        
    public:
        F_MaterialRenderPack(const F_SerializableObjectInitParams& InitParams);
        ~F_MaterialRenderPack() override;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    public:
        void PrepareTemplates(
            const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap
        ) override;
#endif
    };
    
    class ABYTEK_ENGINE_NFC_API F_Material : public A_MaterialInterface
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_MaterialInterface)
        ABYTEK_END_REFLECTOR(F_Material);
        
    private:
        TS<F_MaterialInstance> _MainInstance;
        TS<F_MaterialRenderPack> _RenderPack;
        
        TF_Vector<TW<F_MaterialInstance>> _Instances;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetMainInstance() const noexcept
        {
            return _MainInstance;
        }
        ABYTEK_FORCE_INLINE const auto& GetRenderPack() const noexcept
        {
            return _RenderPack;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetInstances() const noexcept
        {
            return _Instances;
        }
        
    public:
        F_Material(const F_SerializableObjectInitParams& InitParam);
        ~F_Material() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    public:
        TS<F_Material> GetMaterial() const override;
        TS<F_MaterialInstance> GetMaterialInstance() const override;
    };
}