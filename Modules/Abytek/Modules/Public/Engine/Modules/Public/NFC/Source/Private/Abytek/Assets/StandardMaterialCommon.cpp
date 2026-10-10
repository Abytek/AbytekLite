#include "Abytek/Assets/StandardMaterialCommon.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryType.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxy.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetType.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetTypeProxy.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_StandardMaterialShaderParameters)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_StandardMaterialShaderParameters"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Domain);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(ShadingModel);
    }

    void F_StandardMaterialTargetSignature::AssignGeometryFactoryType(const TW<A_GeometryFactoryType>& GeometryFactoryType)
    {
        if (GeometryFactoryType)
        {
            GeometryFactoryTypeHashCode = GeometryFactoryType->GetHashCode();
        }
        else
        {
            GeometryFactoryTypeHashCode = 0;
        }
    }
    void F_StandardMaterialTargetSignature::AssignGeometryFactoryType(const TW<A_GeometryFactoryTypeProxy>& GeometryFactoryTypeProxy)
    {
        if (GeometryFactoryTypeProxy)
        {
            GeometryFactoryTypeHashCode = GeometryFactoryTypeProxy->GetHashCode();
        }
        else
        {
            GeometryFactoryTypeHashCode = 0;
        }
    }
    void F_StandardMaterialTargetSignature::AssignMaterialTargetType(const TW<A_MaterialTargetType>& MaterialTargetType)
    {
        if (MaterialTargetType)
        {
            MaterialTargetTypeHashCode = MaterialTargetType->GetHashCode();
        }
        else
        {
            MaterialTargetTypeHashCode = 0;
        }
    }
    void F_StandardMaterialTargetSignature::AssignMaterialTargetType(const TW<A_MaterialTargetTypeProxy>& MaterialTargetTypeProxy)
    {
        if (MaterialTargetTypeProxy)
        {
            MaterialTargetTypeHashCode = MaterialTargetTypeProxy->GetHashCode();
        }
        else
        {
            MaterialTargetTypeHashCode = 0;
        }
    }

    F_MaterialPermutationHashCode F_StandardMaterialTargetSignature::GetHashCode() const
    {
        return HashCombineU64(
            GeometryFactoryTypeHashCode,
            MaterialTargetTypeHashCode
        );
    }

    F_StandardMaterialTargetSignature F_StandardMaterialCompileTarget::GetSignature() const
    {
        F_StandardMaterialTargetSignature Result;
        Result.AssignGeometryFactoryType(GeometryFactoryType.Weak());
        Result.AssignMaterialTargetType(MaterialTargetType.Weak());
        return Result;
    }

    F_StandardMaterialTargetSignature F_StandardMaterialTarget::GetSignature() const
    {
        F_StandardMaterialTargetSignature Result;
        Result.AssignGeometryFactoryType(GeometryFactoryTypeProxy.Weak());
        Result.AssignMaterialTargetType(MaterialTargetTypeProxy.Weak());
        return Result;
    }

    TS<A_RHIBindGroup> F_StandardMaterialExternalBinding::CreateBindGroup() const
    {
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(IsValid()) << "Invalid binding, cannot create bind group";
        F_RHIBindGroupBuildParams BuildParams;
        BuildParams.Context = RHITemplateRuntime->GetContext();
        BuildParams.TemplateRuntime = RHITemplateRuntime;
        return RACreateAndBuildShared<A_RHIBindGroup>(BuildParams);
    }
    TS<A_RHIPipelineState> F_StandardMaterialExternalPipeline::AcquirePipelineState() const
    {
        return RHITemplateRuntime;
    }

    F_MaterialPermutationHashCode F_StandardMaterialExternalBindingQuery::GetHashCode() const
    {
        return GenerateMaterialExternalBindingHashCode(
            MaterialName,
            MaterialPermutationHashCode,
            MaterialTarget.GetSignature().GetHashCode(),
            Index
        );
    }
    F_StandardMaterialExternalBinding F_StandardMaterialExternalBindingQuery::Instantiate(const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime) const
    {
        F_StandardMaterialExternalBinding Result;
        Result.RHITemplateRuntime = RenderRegistryRuntime->GetOrActivate(GetHashCode()).FastCast<A_RHIBindGroupTemplateRuntime>();
        return ABYTEK_MOVE(Result);
    }
    F_MaterialPermutationHashCode F_StandardMaterialExternalPipelineQuery::GetHashCode() const
    {
        return GenerateMaterialExternalPipelineHashCode(
            MaterialName,
            MaterialPermutationHashCode,
            MaterialTarget.GetSignature().GetHashCode(),
            Index
        );
    }
    F_StandardMaterialExternalPipeline F_StandardMaterialExternalPipelineQuery::Instantiate(const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime) const
    {
        F_StandardMaterialExternalPipeline Result;
        Result.RHITemplateRuntime = RenderRegistryRuntime->GetOrActivate(GetHashCode()).FastCast<A_RHIPipelineStateTemplateRuntime>();
        return ABYTEK_MOVE(Result);
    }
}
