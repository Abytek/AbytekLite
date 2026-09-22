#include "Abytek/Renderer/GPUData/GPUDataComponentType.hpp"
#include "Abytek/Renderer/GPUData/GPUDataStorage.hpp"
#include "Abytek/Renderer/GPUData/GPUDataInstanceSet.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"


namespace Abytek
{
    void F_GPUDataComponentType::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_GPUDataComponentTypeBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _GPUData = BuildParams.GPUData;
        _Name = BuildParams.Name;
        _SRVBinding = BuildParams.SRVBinding;
        _UAVBinding = BuildParams.UAVBinding;
        _SizeInBytes = static_cast<U32>(
            AlignSize(BuildParams.SizeInBytes, BuildParams.AlignmentInBytes)
        );
        _AlignmentInBytes = BuildParams.AlignmentInBytes;
    }
    void F_GPUDataComponentType::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _AlignmentInBytes = 0;
        _SizeInBytes = 0;
        _UAVBinding = {};
        _SRVBinding = {};
        _Name = {};
        _GPUData = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }
}
