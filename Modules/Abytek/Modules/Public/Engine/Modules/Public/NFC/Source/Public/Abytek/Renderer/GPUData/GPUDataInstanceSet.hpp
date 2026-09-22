#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/GPUData/GPUDataCommon.hpp"


namespace Abytek
{
    class F_GPUData;
    class F_GPUDataComponentType;

    struct F_GPUDataInstanceSetBuildParams
    {
        TW<F_GPUData> GPUData;
        U32 Num = 0;
    };
    class ABYTEK_ENGINE_NFC_API F_GPUDataInstanceSet final : public A_RenderObject
    {
    public:
        friend class F_GPUData;
        
    private:
        TW<F_GPUData> _GPUData;
        TF_Optional<F_GPUDataInstanceAllocation> _Allocation;
        U32 _Num = 0;
        
        struct F_UploadCandidate
        {
            U32 ComponentIndex = 0;
            TF_Span<const U8> CachedData;
        };
        TF_SmallVector<F_UploadCandidate, 1> _UploadCandidates;
        B8 _IsDirty = false;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetGPUData() const noexcept
        {
            return _GPUData;
        }
        ABYTEK_FORCE_INLINE const auto& GetAllocation() const noexcept
        {
            return _Allocation;
        }
        ABYTEK_FORCE_INLINE auto GetNum() const noexcept
        {
            return _Num;
        }
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_GPUDataInstanceSet, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_GPUDataInstanceSetBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void UploadComponents(U32 Index, const void* DataPtr);
        void UploadComponents(const TW_Valid<F_GPUDataComponentType>& ComponentType, const void* DataPtr);
        
    private:
        void _MarkDirty();
    };
}
