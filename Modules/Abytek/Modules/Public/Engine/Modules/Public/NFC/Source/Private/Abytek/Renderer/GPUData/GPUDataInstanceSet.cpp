#include "Abytek/Renderer/GPUData/GPUDataInstanceSet.hpp"
#include "Abytek/Renderer/GPUData/GPUDataStorage.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"
#include "Abytek/Renderer/GPUData/GPUDataPage.hpp"
#include "Abytek/RHIHelper.hpp"


namespace Abytek
{
    void F_GPUDataInstanceSet::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_GPUDataInstanceSetBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _GPUData = BuildParams.GPUData; 
        _Num = BuildParams.Num;
        _GPUData->_RegisterInstanceSet(ABYTEK_WTHIS());
        _Allocation = _GPUData->GetStorage()->New(SubmissionItemContainer, _Num);
    }
    void F_GPUDataInstanceSet::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        if (_Allocation)
        {
            _GPUData->GetStorage()->Delete(SubmissionItemContainer, *_Allocation);
        }
        _GPUData->_UnregisterInstanceSet(ABYTEK_WTHIS());
        _Num = 0;
        _GPUData = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void F_GPUDataInstanceSet::UploadComponents(U32 ComponentIndex, const void* DataPtr)
    {
        ABYTEK_ENGINE_NFC_ASSERT(_Allocation) << "Cannot upload component on GPU data instance set having invalid allocation";
        const auto& Allocation = *_Allocation;
        auto Process = H_RHI::GetMainProcess();
        auto ComponentSizeInBytes = _GPUData->GetComponentIndexToSizeInBytes()[ComponentIndex];
        auto CachedData = Process->GetArena()->CacheData({
            ((const U8*)DataPtr),
            ((const U8*)DataPtr) + _Num * ComponentSizeInBytes,
        });
        for (auto& UploadCandidate : _UploadCandidates)
        {
            if (UploadCandidate.ComponentIndex == ComponentIndex)
            {
                UploadCandidate.CachedData = CachedData;
                return;
            }
        }
        F_UploadCandidate UploadCandidate;
        UploadCandidate.ComponentIndex = ComponentIndex;
        UploadCandidate.CachedData = CachedData;
        _UploadCandidates.push_back(UploadCandidate);
    }
    void F_GPUDataInstanceSet::UploadComponents(const TW_Valid<F_GPUDataComponentType>& ComponentType, const void* DataPtr)
    {
        UploadComponents(_GPUData->GetComponentTypeIndex(ComponentType), DataPtr);
    }

    void F_GPUDataInstanceSet::_MarkDirty()
    {
        if (_IsDirty)
        {
            return;
        }
        _IsDirty = true;
        _GPUData->_AddDirtyInstanceSet(ABYTEK_STHIS());
    }
}
