#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/GPUData/GPUDataCommon.hpp"
#include "Abytek/GPUData/Render/GPUData.hpp"
#include "Abytek/GPUData/Render/GPUDataComponentType.hpp"


namespace Abytek
{
    class F_GPUData;
    class F_GPUDataComponentType;

    struct ABYTEK_ENGINE_NFC_API I_GlobalGPUDataComponent
    {
        using F_CreateFunction = TF_Function<void(const TW_Valid<F_GPUData>& GPUData, TS<F_GPUDataComponentType>& OutComponentType, F_GeneralTypeHashCode& OutHashCode)>;
        
        static F_Name GetMetadataElementName_CreateFunction()
        {
            return ABYTEK_NAME("Abytek::I_GlobalGPUDataComponent::Create");
        }
        
        ABYTEK_BEGIN_REFLECTOR()
            ABYTEK_REFLECT_GEN_INTERFACE()
        {
            auto& Metadata = ReflectionType->GetMetadata();
            Metadata.Add(
                GetMetadataElementName_CreateFunction(),
                F_CreateFunction(
                    [](const TW_Valid<F_GPUData>& GPUData, TS<F_GPUDataComponentType>& OutComponentType, F_GeneralTypeHashCode& OutHashCode)
                    {
                        F_GPUDataComponentTypeBuildParams BuildParams;
                        BuildParams.GPUData = GPUData;
                        BuildParams.HashCode = H_GeneralTypeHashCode::MakeStatic<__F_SelfOrInherited>();
                        BuildParams.SizeInBytes = sizeof(__F_SelfOrInherited);
                        BuildParams.AlignmentInBytes = ABYTEK_ALIGNOF(__F_SelfOrInherited);
                        OutComponentType = TS<F_GPUDataComponentType>()(
                            GPUData->GetWorldRenderResource(),
                            BuildParams
                        );
                        OutHashCode = BuildParams.HashCode;
                    }
                )
            );
        }
        ABYTEK_END_REFLECTOR(I_GlobalGPUDataComponent)
    };
}