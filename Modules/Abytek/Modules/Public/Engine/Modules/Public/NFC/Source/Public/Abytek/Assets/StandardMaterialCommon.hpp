#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Assets/Material.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryCommon.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetCommon.hpp"


namespace Abytek
{
    class A_GeometryFactoryTypeProxy;
    class A_GeometryFactoryType;
    class A_MaterialTargetTypeProxy;
    class A_MaterialTargetType;

    enum class E_StandardMaterialDomain : U8
    {
        NONE,
        SURFACE,
        DEFAULT = SURFACE
    };
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_StandardMaterialDomain)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::E_StandardMaterialDomain"));
        
        ABYTEK_REFLECT_ENUM_VALUE(NONE);
        ABYTEK_REFLECT_ENUM_VALUE(SURFACE);
        ABYTEK_REFLECT_ENUM_VALUE(DEFAULT);
    }
    
    enum class E_StandardMaterialShadingModel : U8
    {
        NONE,
        LIT,
        DEFAULT = LIT
    };
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_StandardMaterialShadingModel)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::E_StandardMaterialShadingModel"));
        
        ABYTEK_REFLECT_ENUM_VALUE(NONE);
        ABYTEK_REFLECT_ENUM_VALUE(LIT);
        ABYTEK_REFLECT_ENUM_VALUE(DEFAULT);
    }
    
    struct ABYTEK_ENGINE_NFC_API F_StandardMaterialShaderParameters
    {
        ABYTEK_BEGIN_REFLECTOR(I_Serializable);
        ABYTEK_END_REFLECTOR(F_StandardMaterialShaderParameters);
        
        E_StandardMaterialDomain Domain = E_StandardMaterialDomain::DEFAULT;
        E_StandardMaterialShadingModel ShadingModel = E_StandardMaterialShadingModel::DEFAULT;
    };
    
    struct ABYTEK_ENGINE_NFC_API F_StandardMaterialTargetSignature
    {
        F_GeometryFactoryTypeHashCode GeometryFactoryTypeHashCode = 0;
        F_MaterialTargetTypeHashCode MaterialTargetTypeHashCode = 0;
        
        void AssignGeometryFactoryType(const TW<A_GeometryFactoryType>& GeometryFactoryType);
        void AssignGeometryFactoryType(const TW<A_GeometryFactoryTypeProxy>& GeometryFactoryTypeProxy);
        void AssignMaterialTargetType(const TW<A_MaterialTargetType>& MaterialTargetType);
        void AssignMaterialTargetType(const TW<A_MaterialTargetTypeProxy>& MaterialTargetTypeProxy);
        
        F_MaterialPermutationHashCode GetHashCode() const;
    };
    
    struct ABYTEK_ENGINE_NFC_API F_StandardMaterialCompileTarget
    {
        TS<A_GeometryFactoryType> GeometryFactoryType;
        TS<A_MaterialTargetType> MaterialTargetType;
        
        F_StandardMaterialTargetSignature GetSignature() const;
    };
    
    struct ABYTEK_ENGINE_NFC_API F_StandardMaterialTarget
    {
        TS<A_GeometryFactoryTypeProxy> GeometryFactoryTypeProxy;
        TS<A_MaterialTargetTypeProxy> MaterialTargetTypeProxy;
        
        F_StandardMaterialTargetSignature GetSignature() const;
    };
    
    struct ABYTEK_ENGINE_NFC_API F_StandardMaterialExternalBinding
    {
        TS<A_RHIBindGroupTemplateRuntime> RHITemplateRuntime;
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return static_cast<B8>(RHITemplateRuntime);
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        TS<A_RHIBindGroup> CreateBindGroup() const;
    };
    struct ABYTEK_ENGINE_NFC_API F_StandardMaterialExternalPipeline
    {
        TS<A_RHIPipelineStateTemplateRuntime> RHITemplateRuntime;
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return static_cast<B8>(RHITemplateRuntime);
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        TS<A_RHIPipelineState> AcquirePipelineState() const;
    };
    
    struct ABYTEK_ENGINE_NFC_API F_StandardMaterialExternalBindingQuery
    {
        F_Name MaterialName;
        F_MaterialPermutationHashCode MaterialPermutationHashCode = 0;
        F_StandardMaterialTarget MaterialTarget;
        U32 Index = 0;
        
        F_MaterialPermutationHashCode GetHashCode() const;
        F_StandardMaterialExternalBinding Instantiate(const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime) const;
    };
    struct ABYTEK_ENGINE_NFC_API F_StandardMaterialExternalPipelineQuery
    {
        F_Name MaterialName;
        F_MaterialPermutationHashCode MaterialPermutationHashCode = 0;
        F_StandardMaterialTarget MaterialTarget;
        U32 Index = 0;
        
        F_MaterialPermutationHashCode GetHashCode() const;
        F_StandardMaterialExternalPipeline Instantiate(const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime) const;
    };
}
