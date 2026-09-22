#include "Abytek/Renderer/GPUData/GPUData.hpp"
#include "Abytek/Renderer/GPUData/GPUDataStorage.hpp"
#include "Abytek/Renderer/GPUData/GPUDataInstanceSet.hpp"
#include "Abytek/Renderer/GPUData/GPUDataComponentType.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Renderer/GPUData/GPUDataPage.hpp"


namespace Abytek
{
    void F_GPUData::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_GPUDataBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Scene = BuildParams.Scene;
        
        for (const auto& ComponentTypeConfig : BuildParams.ComponentTypes)
        {
            F_GPUDataComponentTypeBuildParams ComponentTypeBuildParams;
            ComponentTypeBuildParams.GPUData = ABYTEK_WTHIS();
            static_cast<F_GPUDataComponentTypeConfig&>(ComponentTypeBuildParams) = ComponentTypeConfig;
            auto ComponentType = F_GPUDataComponentType::CreateAndInit(
                GetWorldRenderResource(),
                SubmissionItemContainer, 
                ComponentTypeBuildParams
            );
            _ComponentTypes.push_back(ComponentType);
            _ComponentIndexToSizeInBytes.push_back(ComponentTypeConfig.SizeInBytes);
            _ComponentIndexToAlignmentInBytes.push_back(ComponentTypeConfig.AlignmentInBytes);
        }
        
        F_GPUDataStorageBuildParams StorageBuildParams;
        StorageBuildParams.GPUData = ABYTEK_WTHIS();
#ifdef ABYTEK_DEBUG_INFO
        _Storage = F_GPUDataStorage::CreateAndInit_WithDebugName(
            *GetDebugName() + ABYTEK_TEXT(".Storage"),
#else
        _Storage = F_GPUDataStorage::CreateAndInit(
#endif
            GetWorldRenderResource(),
            SubmissionItemContainer, 
            StorageBuildParams
        );
    }
    void F_GPUData::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _Storage->Release(SubmissionItemContainer);
        _Storage = {};
        
        for (const auto& ComponentType : _ComponentTypes)
        {
            ComponentType->Release(SubmissionItemContainer);
        }
        _ComponentTypes = {};
        
        _Scene = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void F_GPUData::BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _CriticalSection(
            [this, &SubmissionItemContainer]
            {
                _Storage->BeginUpdate(SubmissionItemContainer);
            }
        );
    }
    void F_GPUData::EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _CriticalSection(
            [this, &SubmissionItemContainer]
            {
                _Storage->EndUpdate(SubmissionItemContainer);
            }
        );
    }
    void F_GPUData::BeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _CriticalSection(
            [this, &SubmissionItemContainer]
            {
                _FlushDirtyInstanceSets(SubmissionItemContainer);
                _Storage->BeginPostUpdate(SubmissionItemContainer);
            }
        );
    }
    void F_GPUData::EndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _CriticalSection(
            [this, &SubmissionItemContainer]
            {
                _Storage->EndPostUpdate(SubmissionItemContainer);
            }
        );
    }

    void F_GPUData::_RegisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet)
    {
        _CriticalSection(
            [this, &InstanceSet]
            {
                _InstanceSets.insert(InstanceSet);
            }
        );
    }
    void F_GPUData::_UnregisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet)
    {
        _CriticalSection(
            [this, &InstanceSet]
            {
                _InstanceSets.erase(_InstanceSets.find(InstanceSet));
            }
        );
    }

    void F_GPUData::_AddDirtyInstanceSet(const TS<F_GPUDataInstanceSet>& InstanceSet)
    {
        _CriticalSection(
            [this, &InstanceSet]
            {
                _DirtyInstanceSets.insert(InstanceSet);
            }
        );
    }
    void F_GPUData::_FlushDirtyInstanceSets(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        for (const auto& InstanceSet : _DirtyInstanceSets)
        {
            const auto& Allocation = *InstanceSet->GetAllocation();
            for (const auto& UploadCandidate : InstanceSet->_UploadCandidates)
            {
                H_RHISubmissionUtilities::UploadBuffer(
                    SubmissionItemContainer,
                    UploadCandidate.CachedData,
                    Allocation.Page->GetRHIBuffer(UploadCandidate.ComponentIndex),
                    static_cast<U64>(Allocation.BeginLocalIndex) 
                    * static_cast<U64>(_ComponentIndexToSizeInBytes[UploadCandidate.ComponentIndex])
                );
            }
            InstanceSet->_IsDirty = false;
            InstanceSet->_UploadCandidates = {};
        }
    }

    B8 F_GPUData::HasComponentType(const F_Name& Name) const
    {
        for (const auto& ComponentType : _ComponentTypes)
        {
            if (ComponentType->GetName() == Name)
            {
                return true;
            }
        }
        return false;
    }
    U32 F_GPUData::GetComponentTypeIndex(const F_Name& Name) const
    {
        U32 Num = static_cast<U32>(_ComponentTypes.size());
        for (U32 Index = 0; Index < Num; ++Index)
        {
            if (_ComponentTypes[Index]->GetName() == Name)
            {
                return Index;
            }
        }
        ABYTEK_LOG_FATAL() << "Not found component type with name: " << Name;
        return ~U32(0);
    }
    U32 F_GPUData::GetComponentTypeIndex(const TW_Valid<F_GPUDataComponentType>& ComponentType) const
    {
        U32 Num = static_cast<U32>(_ComponentTypes.size());
        for (U32 Index = 0; Index < Num; ++Index)
        {
            if (_ComponentTypes[Index].Weak() == ComponentType)
            {
                return Index;
            }
        }
        ABYTEK_LOG_FATAL() << "Not found component type";
        return ~U32(0);
    }
    TS<F_GPUDataComponentType> F_GPUData::FindComponentType(const F_Name& Name) const
    {
        for (const auto& ComponentType : _ComponentTypes)
        {
            if (ComponentType->GetName() == Name)
            {
                return ComponentType;
            }
        }
        return {};
    }
    TS<F_GPUDataComponentType> F_GPUData::GetComponentType(const F_Name& Name) const
    {
        auto Result = FindComponentType(Name);
        ABYTEK_ENGINE_NFC_ASSERT(Result) << "Not found component type with name: " << Name;
        return Result;
    }
    TS<F_GPUDataComponentType> F_GPUData::GetComponentType(U32 Index) const
    {
        return _ComponentTypes[Index];
    }
}
