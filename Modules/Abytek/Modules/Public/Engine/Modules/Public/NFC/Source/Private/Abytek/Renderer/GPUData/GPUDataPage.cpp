#include "Abytek/Renderer/GPUData/GPUDataPage.hpp"
#include "Abytek/Renderer/GPUData/GPUDataStorage.hpp"


namespace Abytek
{
    void F_GPUDataPage::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW<F_GPUDataStorage>& Storage, 
        U32 Index, 
        U32 SizeInInstances
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Storage = Storage;
        _Index = Index;
        _SizeInInstances = SizeInInstances;
        
        _Distributor.Extends(_SizeInInstances);
        
        const auto& ComponentTypes = Storage->GetGPUData()->GetComponentTypes();
        U32 NumComponentTypes = static_cast<U32>(ComponentTypes.size());
        for (U32 Idx = 0; Idx < NumComponentTypes; ++Idx)
        {
            const auto& ComponentType = ComponentTypes[Idx];
            
            F_RHIBufferBuildParams BufferBuildParams;
            BufferBuildParams.Context = H_RHI::GetMainContext().Weak();
            BufferBuildParams.BufferAspect.SizeInBytes = _SizeInInstances * ComponentType->GetSizeInBytes();
            BufferBuildParams.AccessCapabilities = (
                F_RHIResourceAccess::MakeSRVCapabilities()
                | F_RHIResourceAccess::MakeUAVCapabilities()
                | F_RHIResourceAccess::MakeVertexBufferCapabilities()
                | F_RHIResourceAccess::MakeIndexBufferCapabilities()
            );
            auto RHIBuffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            RHIBuffer->SetDebugName(GetDebugName());
#endif
        
            {
                F_RHIBufferViewBuildParams BufferViewBuildParams;
                BufferViewBuildParams.Context = H_RHI::GetMainContext().Weak();
                BufferViewBuildParams.BufferViewAspect.SizeInBytes = BufferBuildParams.BufferAspect.SizeInBytes;
                BufferViewBuildParams.BufferViewAspect.RawAccess = true;
                BufferViewBuildParams.Resource = RHIBuffer;
                BufferViewBuildParams.Access = F_RHIResourceAccess::MakeSRV();
                auto SRV = RACreateAndBuildShared<A_RHIResourceView>(BufferViewBuildParams);
#ifdef ABYTEK_DEBUG_INFO
                SRV->SetDebugName(*GetDebugName() + ABYTEK_TEXT(".SRV"));
#endif
                _SRVs.push_back(SRV);
            }
            {
                F_RHIBufferViewBuildParams BufferViewBuildParams;
                BufferViewBuildParams.Context = H_RHI::GetMainContext().Weak();
                BufferViewBuildParams.BufferViewAspect.SizeInBytes = BufferBuildParams.BufferAspect.SizeInBytes;
                BufferViewBuildParams.BufferViewAspect.RawAccess = true;
                BufferViewBuildParams.Resource = RHIBuffer;
                BufferViewBuildParams.Access = F_RHIResourceAccess::MakeUAV();
                auto UAV = RACreateAndBuildShared<A_RHIResourceView>(BufferViewBuildParams);
#ifdef ABYTEK_DEBUG_INFO
                UAV->SetDebugName(*GetDebugName() + ABYTEK_TEXT(".UAV"));
#endif
                _UAVs.push_back(UAV);
            }
        }
    }
    void F_GPUDataPage::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _UAVs = {};
        _SRVs = {};
        
        _RHIBuffers = {};
        
        _Distributor = {};
        
        _SizeInInstances = 0;
        _Index = ~U32(0);
        _Storage = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    TF_Optional<F_GPUDataInstanceAllocation> F_GPUDataPage::Allocate(U32 NumInstances)
    {
        if (auto OffsetInInstances = _Distributor.Allocate(1))
        {
            F_GPUDataInstanceAllocation Allocation;
            Allocation.Page = ABYTEK_STHIS();
            Allocation.BeginLocalIndex = *OffsetInInstances;
            Allocation.EndLocalIndex = Allocation.BeginLocalIndex + NumInstances;
            return Allocation;
        }
        return {};
    }
    void F_GPUDataPage::Deallocate(const F_GPUDataInstanceAllocation& Allocation)
    {
        _Distributor.Deallocate(Allocation.BeginLocalIndex);
    }

    TS<A_RHIResource> F_GPUDataPage::GetRHIBuffer(U32 Index) const
    {
        return _RHIBuffers[Index];
    }
    TS<A_RHIResource> F_GPUDataPage::GetRHIBuffer(const F_Name& Name) const
    {
        return _RHIBuffers[_Storage->GetGPUData()->GetComponentTypeIndex(Name)];
    }
    TS<A_RHIResourceView> F_GPUDataPage::GetSRV(U32 Index) const
    {
        return _SRVs[Index];
    }
    TS<A_RHIResourceView> F_GPUDataPage::GetSRV(const F_Name& Name) const
    {
        return _SRVs[_Storage->GetGPUData()->GetComponentTypeIndex(Name)];
    }
    TS<A_RHIResourceView> F_GPUDataPage::GetUAV(U32 Index) const
    {
        return _UAVs[Index];
    }
    TS<A_RHIResourceView> F_GPUDataPage::GetUAV(const F_Name& Name) const
    {
        return _UAVs[_Storage->GetGPUData()->GetComponentTypeIndex(Name)];
    }
}
