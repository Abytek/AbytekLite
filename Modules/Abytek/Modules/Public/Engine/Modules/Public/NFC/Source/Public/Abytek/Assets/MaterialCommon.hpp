#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/RenderPack.hpp"
#include "Abytek/Assets/MaterialInterface.hpp"


namespace Abytek
{
    class F_MaterialInstance;
    class A_Material;
    
    using F_MaterialPermutationHashCode = U64;
    
    inline F_RHITemplateHashCode GenerateMaterialMainBindingHashCode(
        const F_Name& MaterialName
    )
    {
        return H_RenderCore::GenerateTemplateHashCode<A_Material>(
            ABYTEK_NAME("MainBinding").GetHashCode(),
            MaterialName.GetHashCode()
        );
    }
    inline F_RHITemplateHashCode GenerateMaterialExternalBindingHashCode(
        const F_Name& MaterialName,
        F_MaterialPermutationHashCode MaterialPermutationHashCode,
        F_MaterialPermutationHashCode MaterialTargetSignatureHashCode,
        U32 ExternalBindingIndex = 0
    )
    {
        return H_RenderCore::GenerateTemplateHashCode<A_Material>(
            HashCombineU64(
                ABYTEK_NAME("ExternalBinding").GetHashCode(),
                HashCombineU64(
                    MaterialPermutationHashCode, 
                    HashCombineU64(
                        MaterialTargetSignatureHashCode, 
                        U64(ExternalBindingIndex)
                    )
                )
            ),
            MaterialName.GetHashCode()
        );
    }
    inline F_RHITemplateHashCode GenerateMaterialExternalPipelineHashCode(
        const F_Name& MaterialName,
        F_MaterialPermutationHashCode MaterialPermutationHashCode,
        F_MaterialPermutationHashCode MaterialTargetSignatureHashCode,
        U32 ExternalPipelineIndex = 0
    )
    {
        return H_RenderCore::GenerateTemplateHashCode<A_Material>(
            HashCombineU64(
                ABYTEK_NAME("ExternalPipeline").GetHashCode(),
                HashCombineU64(
                    MaterialPermutationHashCode, 
                    HashCombineU64(
                        MaterialTargetSignatureHashCode, 
                        U64(ExternalPipelineIndex)
                    )
                )
            ),
            MaterialName.GetHashCode()
        );
    }

    enum class E_MaterialShaderSourceType : U8
    {
        NONE,
        SLANG,
        
        DEFAULT = SLANG
    };
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_MaterialShaderSourceType)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::E_MaterialShaderSourceType"));
        
        ABYTEK_REFLECT_ENUM_VALUE(NONE);
        ABYTEK_REFLECT_ENUM_VALUE(SLANG);
        ABYTEK_REFLECT_ENUM_VALUE(DEFAULT);
    }
    
    struct F_MaterialShaderSource_Slang
    {
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_MaterialShaderSource_Slang);
        
        F_Name ModuleName;
        
        static F_MaterialShaderSource_Slang Make(const F_Name& InModuleName)
        {
            F_MaterialShaderSource_Slang Result;
            Result.ModuleName = InModuleName;
            return ABYTEK_MOVE(Result);
        }
    };
    struct F_MaterialShaderSource
    {
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_MaterialShaderSource);
        
        E_MaterialShaderSourceType Type = E_MaterialShaderSourceType::NONE;
        F_MaterialShaderSource_Slang Slang;
        
        static F_MaterialShaderSource Make(const F_MaterialShaderSource_Slang& InSlang)
        {
            F_MaterialShaderSource Result;
            Result.Type = E_MaterialShaderSourceType::SLANG;
            Result.Slang = InSlang;
            return ABYTEK_MOVE(Result);
        }
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return (Type != E_MaterialShaderSourceType::NONE);
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
    };
}