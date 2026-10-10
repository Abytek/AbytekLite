#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/RenderPack.hpp"
#include "Abytek/Assets/MaterialInterface.hpp"
#include "Abytek/Development/WorldContextDevelopmentData.hpp"


namespace Abytek
{
    class F_MaterialInstance;
    
    struct F_MaterialCompileBinding
    {
        F_RHIBindGroupTemplateCompileParams RHICompileParams;
    };
    struct F_MaterialCompilePipeline
    {
        F_RHIPipelineStateTemplateCompileParams RHICompileParams;
    };
    struct F_MaterialCompilePermutation
    {
        B8 ShouldCompile = true;
        F_MaterialPermutationHashCode MaterialPermutationHashCode = 0;
        F_MaterialPermutationHashCode TargetSignatureHashCode = 0;
        TF_Vector<F_MaterialCompileBinding> ExternalBindings;
        TF_Vector<F_MaterialCompilePipeline> ExternalPipelines;
    };
    
    class ABYTEK_ENGINE_NFC_API F_MaterialRenderPack : public F_RenderPack
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_RenderPack)
        ABYTEK_END_REFLECTOR(F_MaterialRenderPack);
        
    public:
        friend class A_Material;
     
    private:
        TW<A_Material> _Material;
        
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
            const TS<F_RenderRegistry>& RenderRegistry,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap
        ) override;
#endif
    };
    
    class ABYTEK_ENGINE_NFC_API A_Material : public A_MaterialInterface
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_MaterialInterface)
        ABYTEK_END_REFLECTOR(A_Material);
        
    public:
        friend class F_MaterialRenderPack;
        
    private:
        TS<F_MaterialRenderPack> _RenderPack;
        
        F_MaterialPropertyList _SystemPropertyList;
        F_MaterialPropertyList _UserDefinedPropertyList;
        F_MaterialPropertyList _PropertyList;
        F_MaterialPropertyListLayout _PropertyListLayout;
        F_MaterialPropertyInstanceList _DefaultPropertyInstanceList;
        F_MaterialPermutationHashCode _DefaultPermutationHashCode = 0;
        
        B8 _IsPropertyListLayoutDirty = true;
        B8 _IsShaderDirty = true;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TS<F_WorldContextDevelopmentData> _WorldContextDevelopmentData;
#endif
        
        TF_Vector<TW<F_MaterialInstance>> _Instances;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRenderPack() const noexcept
        {
            return _RenderPack;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetSystemPropertyList() const noexcept
        {
            return _SystemPropertyList;
        }
        ABYTEK_FORCE_INLINE const auto& GetUserDefinedPropertyList() const noexcept
        {
            return _UserDefinedPropertyList;
        }
        ABYTEK_FORCE_INLINE const auto& GetPropertyList() const noexcept
        {
            return _PropertyList;
        }
        ABYTEK_FORCE_INLINE const auto& GetPropertyListLayout() const noexcept
        {
            return _PropertyListLayout;
        }
        ABYTEK_FORCE_INLINE const auto& GetDefaultPropertyInstanceList() const noexcept
        {
            return _DefaultPropertyInstanceList;
        }
        ABYTEK_FORCE_INLINE const auto& GetDefaultPermutationHashCode() const noexcept
        {
            return _DefaultPermutationHashCode;
        }
        
        ABYTEK_FORCE_INLINE const auto& IsPropertyListLayoutDirty() const noexcept
        {
            return _IsPropertyListLayoutDirty;
        }
        ABYTEK_FORCE_INLINE const auto& IsShaderDirty() const noexcept
        {
            return _IsShaderDirty;
        }
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TS<F_WorldContextDevelopmentData> GetWorldContextDevelopmentData() const final
        {
            return _WorldContextDevelopmentData;
        }
#endif
        
        ABYTEK_FORCE_INLINE const auto& GetInstances() const noexcept
        {
            return _Instances;
        }
        
    protected:
        A_Material(const F_SerializableObjectInitParams& InitParam);
        
    public:
        ~A_Material() override;
        
    protected:
        void OnPostConstruct() override;
        void OnPreDestruct() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    public:
        TS<A_Material> GetMaterial() const override;
        
    public:
        void AddSystemProperty_B8(const TF_MaterialPropertyScalar<B8>& Property);
        void AddSystemProperty_U32(const TF_MaterialPropertyScalar<U32>& Property);
        void AddSystemProperty_I32(const TF_MaterialPropertyScalar<I32>& Property);
        void AddSystemProperty_F32(const TF_MaterialPropertyScalar<F32>& Property);
        void AddSystemProperty_Texture(const F_MaterialPropertyTexture& Property);
        
    public:
        void AddUserDefinedProperty_B8(const TF_MaterialPropertyScalar<B8>& Property);
        void AddUserDefinedProperty_U32(const TF_MaterialPropertyScalar<U32>& Property);
        void AddUserDefinedProperty_I32(const TF_MaterialPropertyScalar<I32>& Property);
        void AddUserDefinedProperty_F32(const TF_MaterialPropertyScalar<F32>& Property);
        void AddUserDefinedProperty_Texture(const F_MaterialPropertyTexture& Property);
        
    private:
        void _UpdatePropertyList();
        void _MarkPropertyListLayoutDirty();
        
    protected:
        void MarkShaderDirty();
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        void PrepareMaterial();
        void PrepareMaterialIfDirty();
#endif
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        virtual void GatherCompilePermutations(
            const TS<F_RenderRegistry>& RenderRegistry,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
            const TF_Span<const F_MaterialPropertyInstanceList>& MaterialPermutations,
            TF_Vector<F_MaterialCompilePermutation>& OutList
        );
#endif
        
    protected:
        TS<A_RenderProxy> CreateRenderProxy() override;
        
    protected:
        void OnCreateRenderState() override;
        void OnDestroyRenderState() override;
    };
}