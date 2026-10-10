#include "Abytek/Assets/Material.hpp"
#include "Abytek/Assets/Render/MaterialRenderProxy.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/Assets/MaterialInstance.hpp"


namespace Abytek
{
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
        const TS<F_RenderRegistry>& RenderRegistry,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap
    )
    {
        auto TemplateDatabase = RenderRegistry->GetTemplateDatabase();
        
        TF_Set<F_RHITemplateHashCode> TemplateHashCodes;
        TF_Set<F_RHITemplateHashCode> TemplateHashCodesToCompile;
        TF_Vector<TF_Function<void(TF_Vector<TS<A_RHITemplate>>& OutTemplates)>> Commands_CompileBinding;
        TF_Vector<TF_Function<void(TF_Vector<TS<A_RHITemplate>>& OutTemplates)>> Commands_CompilePipeline;
        
        F_RHIBindGroupTemplateCompileParams BindGroupTemplateCompileParams;
        BindGroupTemplateCompileParams.Database = TemplateDatabase;
        BindGroupTemplateCompileParams.HashCode = GenerateMaterialMainBindingHashCode(
            _Material->GetName()
        );
        RenderPackTemplateMap->TryBuildCommand(
            RenderRegistry,
            false,
            BindGroupTemplateCompileParams,
            Commands_CompileBinding,
            TemplateHashCodesToCompile,
            TemplateHashCodes,
            *_Material->GetName() + ABYTEK_TEXT(".MainBinding")
        );
        
        TF_Vector<F_MaterialPropertyInstanceList> MaterialPermutations = H_MaterialProperty::GeneratePermutations(_Material->GetPropertyList());
        TF_Vector<F_MaterialCompilePermutation> MaterialCompilePermutations;
        _Material->GatherCompilePermutations(
            RenderRegistry,
            RenderPackTemplateMap,
            MaterialPermutations,
            MaterialCompilePermutations
        );
        for (auto& MaterialCompilePermutation : MaterialCompilePermutations)
        {
            U32 NumExternalBindings = static_cast<U32>(MaterialCompilePermutation.ExternalBindings.size());
            for (U32 ExternalBindingIndex = 0; ExternalBindingIndex < NumExternalBindings; ++ExternalBindingIndex)
            {
                auto& ExternalBinding = MaterialCompilePermutation.ExternalBindings[ExternalBindingIndex];
                ExternalBinding.RHICompileParams.Database = TemplateDatabase;
                ExternalBinding.RHICompileParams.HashCode = GenerateMaterialExternalBindingHashCode(
                    _Material->GetName(),
                    MaterialCompilePermutation.MaterialPermutationHashCode,
                    MaterialCompilePermutation.TargetSignatureHashCode,
                    ExternalBindingIndex
                );
                ExternalBinding.RHICompileParams.CustomBaseDependencyHashCode = GetBaseDependencyHashCodeForTemplates();
                RenderPackTemplateMap->TryBuildCommand(
                    RenderRegistry,
                    false,
                    ExternalBinding.RHICompileParams,
                    Commands_CompileBinding,
                    TemplateHashCodesToCompile,
                    TemplateHashCodes,
                    *_Material->GetName() + ABYTEK_TEXT(".ExternalBinding")
                );
            }
            
            U32 NumExternalPipelines = static_cast<U32>(MaterialCompilePermutation.ExternalPipelines.size());
            for (U32 ExternalPipelineIndex = 0; ExternalPipelineIndex < NumExternalPipelines; ++ExternalPipelineIndex)
            {
                auto& ExternalPipeline = MaterialCompilePermutation.ExternalPipelines[ExternalPipelineIndex];
                ExternalPipeline.RHICompileParams.Database = TemplateDatabase;
                ExternalPipeline.RHICompileParams.HashCode = GenerateMaterialExternalPipelineHashCode(
                    _Material->GetName(),
                    MaterialCompilePermutation.MaterialPermutationHashCode,
                    MaterialCompilePermutation.TargetSignatureHashCode,
                    ExternalPipelineIndex
                );
                ExternalPipeline.RHICompileParams.CustomBaseDependencyHashCode = GetBaseDependencyHashCodeForTemplates();
                RenderPackTemplateMap->TryBuildCommand(
                    RenderRegistry,
                    false,
                    ExternalPipeline.RHICompileParams,
                    Commands_CompilePipeline,
                    TemplateHashCodesToCompile,
                    TemplateHashCodes,
                    *_Material->GetName() + ABYTEK_TEXT(".ExternalPipeline")
                );
            }
        }
        
        ExecuteExclusiveTemplateCompilation(
            RenderRegistry,
            RenderPackTemplateMap,
            TemplateHashCodesToCompile,
            TemplateHashCodes,
            [&](TF_Vector<TS<A_RHITemplate>>& OutNewTemplates)
            {
                ExecuteParallelCompileCommands(Commands_CompileBinding, OutNewTemplates);
                ExecuteParallelCompileCommands(Commands_CompilePipeline, OutNewTemplates);
            }
        );
    }
#endif
    
    ABYTEK_REFLECT(A_Material)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Material"));    
        
        ABYTEK_REFLECT_PROPERTY(_RenderPack);
        ABYTEK_REFLECT_PROPERTY(_SystemPropertyList);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_UserDefinedPropertyList);
        ABYTEK_REFLECT_PROPERTY(_PropertyList);
    }
    
    A_Material::A_Material(const F_SerializableObjectInitParams& InitParam) :
        A_MaterialInterface(InitParam)
    {
        _RenderPack = CreateSerializableSubobjectDelayLoading<F_MaterialRenderPack>(
            ABYTEK_NAME("RenderPack")    
        );
        _RenderPack->_Material = ABYTEK_WTHIS();
        
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            ABYTEK_ENGINE_NFC_ASSERT(GetPackageName()) << "Requires package name for non-CDO material objects";
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
            _WorldContextDevelopmentData = CreateSerializableSubobjectDelayLoading<F_WorldContextDevelopmentData>(
                ABYTEK_NAME("WorldContextDevelopmentData")
            );
#endif
        }
    }
    A_Material::~A_Material()
    {
    }

    void A_Material::OnPostConstruct()
    {
        A_MaterialInterface::OnPostConstruct();
    }
    void A_Material::OnPreDestruct()
    {
        A_MaterialInterface::OnPreDestruct();
    }

    void A_Material::OnLoad()
    {
        A_MaterialInterface::OnLoad();
        _UpdatePropertyList();
        PrepareMaterial();
    }
    void A_Material::OnUnload()
    {
        A_MaterialInterface::OnUnload();
    }

    TS<A_Material> A_Material::GetMaterial() const
    {
        return ABYTEK_STHIS_MUTABLE();
    }

    void A_Material::AddSystemProperty_B8(const TF_MaterialPropertyScalar<B8>& Property)
    {
        _SystemPropertyList.Properties_B8.push_back(Property);
        _MarkPropertyListLayoutDirty();
    }
    void A_Material::AddSystemProperty_U32(const TF_MaterialPropertyScalar<U32>& Property)
    {
        _SystemPropertyList.Properties_U32.push_back(Property);
        _MarkPropertyListLayoutDirty();
    }
    void A_Material::AddSystemProperty_I32(const TF_MaterialPropertyScalar<I32>& Property)
    {
        _SystemPropertyList.Properties_I32.push_back(Property);
        _MarkPropertyListLayoutDirty();
    }
    void A_Material::AddSystemProperty_F32(const TF_MaterialPropertyScalar<F32>& Property)
    {
        _SystemPropertyList.Properties_F32.push_back(Property);
        _MarkPropertyListLayoutDirty();
    }
    void A_Material::AddSystemProperty_Texture(const F_MaterialPropertyTexture& Property)
    {
        _SystemPropertyList.Properties_Texture.push_back(Property);
        _MarkPropertyListLayoutDirty();
    }

    void A_Material::AddUserDefinedProperty_B8(const TF_MaterialPropertyScalar<B8>& Property)
    {
        _UserDefinedPropertyList.Properties_B8.push_back(Property);
        _MarkPropertyListLayoutDirty();
    }
    void A_Material::AddUserDefinedProperty_U32(const TF_MaterialPropertyScalar<U32>& Property)
    {
        _UserDefinedPropertyList.Properties_U32.push_back(Property);
        _MarkPropertyListLayoutDirty();
    }
    void A_Material::AddUserDefinedProperty_I32(const TF_MaterialPropertyScalar<I32>& Property)
    {
        _UserDefinedPropertyList.Properties_I32.push_back(Property);
        _MarkPropertyListLayoutDirty();
    }
    void A_Material::AddUserDefinedProperty_F32(const TF_MaterialPropertyScalar<F32>& Property)
    {
        _UserDefinedPropertyList.Properties_F32.push_back(Property);
        _MarkPropertyListLayoutDirty();
    }
    void A_Material::AddUserDefinedProperty_Texture(const F_MaterialPropertyTexture& Property)
    {
        _UserDefinedPropertyList.Properties_Texture.push_back(Property);
        _MarkPropertyListLayoutDirty();
    }

    void A_Material::_UpdatePropertyList()
    {
        _PropertyList = F_MaterialPropertyList::Combine(_SystemPropertyList, _UserDefinedPropertyList);
        _PropertyListLayout = F_MaterialPropertyListLayout::Make(_PropertyList);
        _DefaultPropertyInstanceList = H_MaterialProperty::GenerateDefaultInstance(_PropertyList);
        _DefaultPermutationHashCode = _DefaultPropertyInstanceList.CalculateHashCode(
            _PropertyList,
            _PropertyListLayout
        );
    }
    void A_Material::_MarkPropertyListLayoutDirty()
    {
        _IsPropertyListLayoutDirty = true;
    }

    void A_Material::MarkShaderDirty()
    {
        _IsShaderDirty = true;
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void A_Material::PrepareMaterial()
    {
        _UpdatePropertyList();
        _RenderPack->PrepareTemplates(
            _RenderPack->GetRegistry(),
            _RenderPack.Weak()
        );
    }
    void A_Material::PrepareMaterialIfDirty()
    {
        if (
            !_IsPropertyListLayoutDirty
            && !_IsShaderDirty
        )
        {
            return;
        }
        PrepareMaterial();
    }
#endif

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void A_Material::GatherCompilePermutations(
        const TS<F_RenderRegistry>& RenderRegistry,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
        const TF_Span<const F_MaterialPropertyInstanceList>& MaterialPermutations,
        TF_Vector<F_MaterialCompilePermutation>& OutList
    )
    {
    }
#endif

    TS<A_RenderProxy> A_Material::CreateRenderProxy()
    {
        return TS<F_MaterialRenderProxy>()(ABYTEK_WTHIS());
    }

    void A_Material::OnCreateRenderState()
    {
        A_MaterialInterface::OnCreateRenderState();
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                CastedRenderProxy = GetRenderProxy().FastCast<F_MaterialRenderProxy>(),
                CachedRenderPackData = _RenderPack->GetMainData()
            ]
            {
                CastedRenderProxy->_RenderPackData = CachedRenderPackData;
            }
        );
    }
    void A_Material::OnDestroyRenderState()
    {
        A_MaterialInterface::OnDestroyRenderState();
    }
}
