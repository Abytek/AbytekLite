#include "Abytek/Assets/StandardMaterial.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryType.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetType.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_StandardMaterial)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_StandardMaterial"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_ShaderParameters);
    }
    
    F_StandardMaterial::F_StandardMaterial(const F_SerializableObjectInitParams& InitParam) :
        A_Material(InitParam)
    {
        _VertexShaderSource = F_MaterialShaderSource::Make(
            F_MaterialShaderSource_Slang::Make(
                ABYTEK_NAME("Abytek/Standard/StandardMaterial/DefaultVertexShader.slangh")    
            )  
        );
        _PixelShaderSource = F_MaterialShaderSource::Make(
            F_MaterialShaderSource_Slang::Make(
                ABYTEK_NAME("Abytek/Standard/StandardMaterial/DefaultPixelShader.slangh")    
            )  
        );
    }
    F_StandardMaterial::~F_StandardMaterial() 
    {
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void F_StandardMaterial::UpdateShaderParameters(const F_StandardMaterialShaderParameters& ShaderParameters)
    {
        _ShaderParameters = ShaderParameters;
        MarkShaderDirty();
    }

    void F_StandardMaterial::UpdateVertexShaderSource(const F_MaterialShaderSource& ShaderSource)
    {
        _VertexShaderSource = ShaderSource;
        MarkShaderDirty();
    }
    void F_StandardMaterial::UpdatePixelShaderSource(const F_MaterialShaderSource& ShaderSource)
    {
        _PixelShaderSource = ShaderSource;
        MarkShaderDirty();
    }
#endif

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void F_StandardMaterial::GatherCompilePermutations(
        const TS<F_RenderRegistry>& RenderRegistry,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
        const TF_Span<const F_MaterialPropertyInstanceList>& MaterialPermutations,
        TF_Vector<F_MaterialCompilePermutation>& OutList
    )
    {
        auto TemplateDatabase = RenderRegistry->GetTemplateDatabase();
        
        auto Material = ABYTEK_STHIS();
        
        for (const auto& MaterialPermutation : MaterialPermutations)
        {
            auto MaterialPermutationHashCode = MaterialPermutation.CalculateHashCode(
                GetPropertyList(),  
                GetPropertyListLayout()  
            );
            
            auto Do_GeometryFactoryType = [&](const TS<A_GeometryFactoryType>& GeometryFactoryType)
            {
                auto Do_MaterialTargetType = [&](const TS<A_MaterialTargetType>& MaterialTargetType)
                {
                    F_StandardMaterialCompileTarget MaterialCompileTarget;
                    MaterialCompileTarget.GeometryFactoryType = GeometryFactoryType;
                    MaterialCompileTarget.MaterialTargetType = MaterialTargetType;
                    
                    F_StandardMaterialTargetSignature MaterialTargetSignature = MaterialCompileTarget.GetSignature();
                    auto MaterialTargetSignatureHashCode = MaterialTargetSignature.GetHashCode();
                
                    F_MaterialCompilePermutation CompilePermutation;
                    CompilePermutation.MaterialPermutationHashCode = MaterialPermutationHashCode;
                    CompilePermutation.TargetSignatureHashCode = MaterialTargetSignatureHashCode;
                    
                    if (GeometryFactoryType)
                    {
                        if (
                            !GeometryFactoryType->ShouldCompilePermutation(
                                RenderRegistry,
                                RenderPackTemplateMap,
                                Material,
                                MaterialPermutation,
                                MaterialCompileTarget
                            )
                        )
                        {
                            return;
                        }
                    }
                    if (MaterialTargetType)
                    {
                        if (
                            !MaterialTargetType->ShouldCompilePermutation(
                                RenderRegistry,
                                RenderPackTemplateMap,
                                Material,
                                MaterialPermutation,
                                MaterialCompileTarget
                            )
                        )
                        {
                            return;
                        }
                    }
                    
                    if (MaterialTargetType)
                    {
                        MaterialTargetType->SetupCompilePermutation(
                            RenderRegistry,
                            RenderPackTemplateMap,
                            Material,
                            MaterialPermutation,
                            MaterialCompileTarget,
                            CompilePermutation
                        );
                        if (!CompilePermutation.ShouldCompile)
                        {
                            return;
                        }
                    }
                    if (GeometryFactoryType)
                    {
                        GeometryFactoryType->SetupCompilePermutation(
                            RenderRegistry,
                            RenderPackTemplateMap,
                            Material,
                            MaterialPermutation,
                            MaterialCompileTarget,
                            CompilePermutation
                        );
                        if (!CompilePermutation.ShouldCompile)
                        {
                            return;
                        }
                    }
                
                    OutList.push_back(CompilePermutation);
                };
                if (
                    ShouldUseMaterialTargetType(
                        RenderRegistry,
                        RenderPackTemplateMap,
                        MaterialPermutation
                    )
                )
                {
                    A_MaterialTargetType::ForEachMaterialTargetType(
                        ABYTEK_WTHIS(),
                        Do_MaterialTargetType
                    );
                }
                else
                {
                    Do_MaterialTargetType({});
                }
            };
            if (
                ShouldUseGeometryFactoryType(
                    RenderRegistry,
                    RenderPackTemplateMap,
                    MaterialPermutation
                )
            )
            {
                A_GeometryFactoryType::ForEachGeometryFactoryType(
                    ABYTEK_WTHIS(),
                    Do_GeometryFactoryType
                );
            }
            else
            {
                Do_GeometryFactoryType({});
            }
        }
    }
    B8 F_StandardMaterial::ShouldUseGeometryFactoryType(
        const TS<F_RenderRegistry>& RenderRegistry,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
        const F_MaterialPropertyInstanceList& MaterialPermutation
    )
    {
        return (_ShaderParameters.Domain == E_StandardMaterialDomain::SURFACE);
    }
    B8 F_StandardMaterial::ShouldUseMaterialTargetType(
        const TS<F_RenderRegistry>& RenderRegistry,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
        const F_MaterialPropertyInstanceList& MaterialPermutation
    )
    {
        return true;
    }
#endif
}
