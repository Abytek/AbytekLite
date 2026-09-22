#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/GPUData/GPUDataCommon.hpp"


namespace Abytek
{
    class F_GPUDataStorage;

    class ABYTEK_ENGINE_NFC_API F_GPUDataPage : public A_RenderObject
    {
    private:
        TW<F_GPUDataStorage> _Storage;
        U32 _Index = ~U32(0);
        U32 _SizeInInstances = 0;
        F_LinearAllocationDistributor _Distributor;
        TF_Vector<TS<A_RHIResource>> _RHIBuffers;
        TF_Vector<TS<A_RHIResourceView>> _SRVs;
        TF_Vector<TS<A_RHIResourceView>> _UAVs;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetStorage() const noexcept
        {
            return _Storage;
        }
        ABYTEK_FORCE_INLINE auto GetIndex() const noexcept
        {
            return _Index;
        }
        ABYTEK_FORCE_INLINE const auto& GetSizeInInstances() const noexcept
        {
            return _SizeInInstances;
        }
        ABYTEK_FORCE_INLINE const auto& GetDistributor() const noexcept
        {
            return _Distributor;
        }
        ABYTEK_FORCE_INLINE const auto& GetRHIBuffers() const noexcept
        {
            return _RHIBuffers;
        }
        ABYTEK_FORCE_INLINE const auto& GetSRVs() const noexcept
        {
            return _SRVs;
        }
        ABYTEK_FORCE_INLINE const auto& GetUAVs() const noexcept
        {
            return _UAVs;
        }
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_GPUDataPage, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TW<F_GPUDataStorage>& Storage, 
            U32 Index, 
            U32 SizeInInstances
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    
    public:
        TF_Optional<F_GPUDataInstanceAllocation> Allocate(U32 NumInstances);
        void Deallocate(const F_GPUDataInstanceAllocation& Allocation);
        
    public:
        TS<A_RHIResource> GetRHIBuffer(U32 Index) const;
        TS<A_RHIResource> GetRHIBuffer(const F_Name& Name) const;
        TS<A_RHIResourceView> GetSRV(U32 Index) const;
        TS<A_RHIResourceView> GetSRV(const F_Name& Name) const;
        TS<A_RHIResourceView> GetUAV(U32 Index) const;
        TS<A_RHIResourceView> GetUAV(const F_Name& Name) const;
    };
}
