#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Assets/Material.hpp"
#include "Abytek/Assets/StandardMaterialCommon.hpp"


namespace Abytek
{
    class A_GeometryFactoryType;

    class ABYTEK_ENGINE_NFC_API F_StandardMaterial : public A_Material
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_Material)
        ABYTEK_END_REFLECTOR(F_StandardMaterial);
        
    private:
        F_StandardMaterialShaderParameters _ShaderParameters;
        
        F_MaterialShaderSource _VertexShaderSource;
        F_MaterialShaderSource _PixelShaderSource;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetShaderParameters() const noexcept
        {
            return _ShaderParameters;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetVertexShaderSource() const noexcept
        {
            return _VertexShaderSource;
        }
        ABYTEK_FORCE_INLINE const auto& GetPixelShaderSource() const noexcept
        {
            return _PixelShaderSource;
        }
        
    public:
        F_StandardMaterial(const F_SerializableObjectInitParams& InitParam);
        ~F_StandardMaterial() override;
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        void UpdateShaderParameters(const F_StandardMaterialShaderParameters& ShaderParameters);
        
        void UpdateVertexShaderSource(const F_MaterialShaderSource& ShaderSource);
        void UpdatePixelShaderSource(const F_MaterialShaderSource& ShaderSource);
#endif
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        void GatherCompilePermutations(
            const TS<F_RenderRegistry>& RenderRegistry,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
            const TF_Span<const F_MaterialPropertyInstanceList>& MaterialPermutations,
            TF_Vector<F_MaterialCompilePermutation>& OutList
        ) override;
        B8 ShouldUseGeometryFactoryType(
            const TS<F_RenderRegistry>& RenderRegistry,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
            const F_MaterialPropertyInstanceList& MaterialPermutation
        );
        B8 ShouldUseMaterialTargetType(
            const TS<F_RenderRegistry>& RenderRegistry,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
            const F_MaterialPropertyInstanceList& MaterialPermutation
        );
#endif
    };
}