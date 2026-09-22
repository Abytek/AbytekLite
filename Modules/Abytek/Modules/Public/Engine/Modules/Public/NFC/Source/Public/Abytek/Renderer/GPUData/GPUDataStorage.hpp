#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/GPUData/GPUDataCommon.hpp"


namespace Abytek
{
    class F_GPUData;
    class F_GPUDataPage;
    class F_GPUDataComponentType;
    
    struct F_GPUDataStorageBuildParams
    {
        TW<F_GPUData> GPUData;
    };
    
    class ABYTEK_ENGINE_NFC_API F_GPUDataStorage final : public A_RenderObject
    {
    private:
        TW<F_GPUData> _GPUData;
        U32 _CapacityLimitInInstances = 0;
        U32 _MinimalPageSizeInInstances = 0;
        U32 _NextPageSizeInInstances = 0;
        U32 _CapacityInInstances = 0;
        TF_Vector<TS<F_GPUDataPage>> _Pages;
        B8 _ShouldRecreateBindGroups = false;
        TF_Vector<TS<A_RHIBindGroup>> _GlobalSRVBindGroups;
        TF_Vector<TS<A_RHIBindGroup>> _GlobalUAVBindGroups;
        F_YieldReentrantCriticalSection _CriticalSection;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetGPUData() const noexcept
        {
            return _GPUData;
        }
        ABYTEK_FORCE_INLINE auto GetCapacityLimitInInstances() const noexcept
        {
            return _CapacityLimitInInstances;
        }
        ABYTEK_FORCE_INLINE auto GetMinimalPageSizeInInstances() const noexcept
        {
            return _MinimalPageSizeInInstances;
        }
        ABYTEK_FORCE_INLINE auto GetNextPageSizeInInstances() const noexcept
        {
            return _NextPageSizeInInstances;
        }
        ABYTEK_FORCE_INLINE auto GetCapacityInInstances() const noexcept
        {
            return _CapacityInInstances;
        }
        ABYTEK_FORCE_INLINE const auto& GetPages() const noexcept
        {
            return _Pages;
        }
        ABYTEK_FORCE_INLINE const auto& GetGlobalSRVBindGroups() const noexcept
        {
            return _GlobalSRVBindGroups;
        }
        ABYTEK_FORCE_INLINE const auto& GetGlobalUAVBindGroups() const noexcept
        {
            return _GlobalUAVBindGroups;
        }
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_GPUDataStorage, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_GPUDataStorageBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
         
    public:
        void BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void BeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    private:
        TF_ConcurrentQueue<F_GPUDataInstanceAllocation> _DeallocationQueue;
        void _FlushDeallocationQueue(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        TF_Optional<F_GPUDataInstanceAllocation> Allocate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, U32 NumInstances);
        TS<F_GPUDataPage> AddNewPage(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, U32 SizeInInstances);
        
    private:
        void _RecreateBindGroupsIfNeeded(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        TF_Optional<F_GPUDataInstanceAllocation> New(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, U32 NumInstances = 1);
        void Delete(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const F_GPUDataInstanceAllocation& Allocation);
        
    public:
        TS<A_RHIBindGroup> GetGlobalSRVBindGroup(U32 Index) const;
        TS<A_RHIBindGroup> GetGlobalSRVBindGroup(const F_Name& Name) const;
        TS<A_RHIBindGroup> GetGlobalUAVBindGroup(U32 Index) const;
        TS<A_RHIBindGroup> GetGlobalUAVBindGroup(const F_Name& Name) const;
    };
}
