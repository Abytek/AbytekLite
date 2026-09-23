#include "Abytek/Renderer/GPUData/GPUDataStorage.hpp"
#include "Abytek/Renderer/GPUData/GPUDataPage.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"


namespace Abytek
{
    void F_GPUDataStorage::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_GPUDataStorageBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _GPUData = BuildParams.GPUData;
        
        _CapacityLimitInInstances = 2 * 1024 * 1024;
        _MinimalPageSizeInInstances = 4 * 1024;
        _NextPageSizeInInstances = _MinimalPageSizeInInstances;
        
        AddNewPage(SubmissionItemContainer, 0);
        _RecreateBindGroupsIfNeeded(SubmissionItemContainer);
    }
    void F_GPUDataStorage::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _GlobalUAVBindGroups = {};
        _GlobalSRVBindGroups = {};
        
        for (const auto& Page : _Pages)
        {
            Page->Release(SubmissionItemContainer);
        }
        _Pages = {};
        
        _GPUData = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void F_GPUDataStorage::BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void F_GPUDataStorage::EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void F_GPUDataStorage::BeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _RecreateBindGroupsIfNeeded(SubmissionItemContainer);
        _FlushDeallocationQueue(SubmissionItemContainer);
    }
    void F_GPUDataStorage::EndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }

    void F_GPUDataStorage::_FlushDeallocationQueue(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        F_GPUDataInstanceAllocation Allocation;
        while (_DeallocationQueue.TryPop(Allocation))
        {
            Allocation.Page->Deallocate(Allocation);
        }
    }

    TF_Optional<F_GPUDataInstanceAllocation> F_GPUDataStorage::Allocate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, U32 NumInstances)
    {
        TF_Optional<F_GPUDataInstanceAllocation> Result;
        _CriticalSection(
            [this, &SubmissionItemContainer, &Result, NumInstances]
            {
                for (const auto& Page : _Pages)
                {
                    if (auto Allocation = Page->Allocate(NumInstances))
                    {
                        Result = ABYTEK_MOVE(Allocation);
                    }
                }
                if (auto Page = AddNewPage(SubmissionItemContainer, NumInstances))
                {
                    if (auto Allocation = Page->Allocate(NumInstances))
                    {
                        Result = ABYTEK_MOVE(Allocation);
                    }
                }
            }
        );
        return Result;
    }
    TS<F_GPUDataPage> F_GPUDataStorage::AddNewPage(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, U32 SizeInInstances)
    {
        U32 ActualSizeInInstances = SizeInInstances + sizeof(F_Vector4_F32); // with alignment
        
        U32 NewPageSizeInInstances = _NextPageSizeInInstances;
        while (NewPageSizeInInstances <= ActualSizeInInstances)
        {
            NewPageSizeInInstances *= 2;
        }
        
        U32 NewCapacityInInstances = NewPageSizeInInstances + _CapacityInInstances;
        if (NewCapacityInInstances > _CapacityLimitInInstances)
        {
            return {};
        }
        
        _NextPageSizeInInstances = NewPageSizeInInstances * 2;
        _CapacityInInstances = NewCapacityInInstances;
        
        _ShouldRecreateBindGroups = true;
        
#ifdef ABYTEK_DEBUG_INFO
        auto Page = F_GPUDataPage::CreateAndInit_WithDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".Pages[") 
            + ToText(_Pages.size()) 
            + ABYTEK_TEXT("]"),
#else
        auto Page = F_GPUDataPage::CreateAndInit(
#endif
            GetWorldRenderResource(),
            SubmissionItemContainer,
            ABYTEK_WTHIS(),
            static_cast<U32>(_Pages.size()),
            NewPageSizeInInstances
        );
        _Pages.push_back(Page);
        return Page;
    }

    void F_GPUDataStorage::_RecreateBindGroupsIfNeeded(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        if (!_ShouldRecreateBindGroups)
        {
            return;
        }
        _ShouldRecreateBindGroups = false;
        
        _GlobalSRVBindGroups = {};
        _GlobalUAVBindGroups = {};
        
        auto DataName = _GPUData->GetName();
        
        const auto& RHIFeatureSupports = GetRHIFeatureSupports();
        
        const auto& ComponentTypes = _GPUData->GetComponentTypes();
        U32 NumComponentTypes = static_cast<U32>(ComponentTypes.size());
        for (U32 Idx = 0; Idx < NumComponentTypes; ++Idx)
        {
            const auto& ComponentType = ComponentTypes[Idx];
            
            auto SlotName = F_GPUDataComponentType::GetBindGroupSlotName(
                DataName, 
                ComponentType->GetName(), 
                RHIFeatureSupports
            );
            
            auto GlobalSRVBindGroup = ComponentType->GetSRVBinding().CreateBindGroup();
            ABYTEK_ENGINE_NFC_ASSERT(GlobalSRVBindGroup->HasSlot(SlotName));
            {
                F_RHIResourceViewSet GPUDataPages;
                for (const auto& Page : _Pages)
                {
                    GPUDataPages.push_back(Page->GetSRV(Idx));
                }
                GlobalSRVBindGroup->BindResourceViewSet(
                    SlotName,
                    GPUDataPages
                );
                GlobalSRVBindGroup->Commit();
#ifdef ABYTEK_DEBUG_INFO
                GlobalSRVBindGroup->SetDebugName(*GetDebugName() + ABYTEK_TEXT(".GlobalSRVBindGroups.") + *ComponentType->GetName());
#endif
            }
            _GlobalSRVBindGroups.push_back(GlobalSRVBindGroup);
        
            auto GlobalUAVBindGroup = ComponentType->GetUAVBinding().CreateBindGroup();
            ABYTEK_ENGINE_NFC_ASSERT(GlobalUAVBindGroup->HasSlot(SlotName));
            {
                F_RHIResourceViewSet GPUDataPages;
                for (const auto& Page : _Pages)
                {
                    GPUDataPages.push_back(Page->GetUAV(Idx));
                }
                GlobalUAVBindGroup->BindResourceViewSet(
                    SlotName,
                    GPUDataPages
                );
                GlobalUAVBindGroup->Commit();
#ifdef ABYTEK_DEBUG_INFO
                GlobalUAVBindGroup->SetDebugName(*GetDebugName() + ABYTEK_TEXT(".GlobalUAVBindGroups.") + *ComponentType->GetName());
#endif
            }
            _GlobalUAVBindGroups.push_back(GlobalUAVBindGroup);
        }
    }

    TF_Optional<F_GPUDataInstanceAllocation> F_GPUDataStorage::New(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, U32 NumInstances)
    {
        return Allocate(SubmissionItemContainer, NumInstances);
    }
    void F_GPUDataStorage::Delete(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const F_GPUDataInstanceAllocation& Allocation)
    {
        _DeallocationQueue.Push(Allocation);
    }

    TS<A_RHIBindGroup> F_GPUDataStorage::GetGlobalSRVBindGroup(U32 Index) const
    {
        return _GlobalSRVBindGroups[Index];
    }
    TS<A_RHIBindGroup> F_GPUDataStorage::GetGlobalSRVBindGroup(const F_Name& Name) const
    {
        return _GlobalSRVBindGroups[_GPUData->GetComponentTypeIndex(Name)];
    }
    TS<A_RHIBindGroup> F_GPUDataStorage::GetGlobalUAVBindGroup(U32 Index) const
    {
        return _GlobalUAVBindGroups[Index];
    }
    TS<A_RHIBindGroup> F_GPUDataStorage::GetGlobalUAVBindGroup(const F_Name& Name) const
    {
        return _GlobalUAVBindGroups[_GPUData->GetComponentTypeIndex(Name)];
    }
}
