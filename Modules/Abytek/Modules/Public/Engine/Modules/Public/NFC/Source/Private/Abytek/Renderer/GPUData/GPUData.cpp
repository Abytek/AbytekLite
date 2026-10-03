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
        _Name = BuildParams.Name;
        
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
            _ComponentIndexToClass.push_back(ComponentTypeConfig.Class);
        }
        
        _InstanceSetHeaderBinding = BuildParams.InstanceSetHeaderBinding;
        
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
        _InstanceSetHeaderBindGroup = {};
        _InstanceSetHeaderSRV = {};
        _InstanceSetHeaderBuffer = {};
        
        _Storage->Release(SubmissionItemContainer);
        _Storage = {};
        
        _InstanceSetHeaderBinding = {};
        
        for (const auto& ComponentType : _ComponentTypes)
        {
            ComponentType->Release(SubmissionItemContainer);
        }
        _ComponentTypes = {};
        
        _Name = {};
        _Scene = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void F_GPUData::BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _CriticalSection(
            [this, &SubmissionItemContainer]
            {
                _IsUpdatePhase.test_and_set(boost::memory_order_release);
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
                _FlushDirtyInstanceSets(SubmissionItemContainer);
                _UpdateInstanceSetHeaderBuffer(SubmissionItemContainer);
                _IsUpdatePhase.clear(boost::memory_order_release);
            }
        );
    }
    void F_GPUData::FinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _CriticalSection(
            [this, &SubmissionItemContainer]
            {
                _Storage->FinalizeFrame(SubmissionItemContainer);
            }
        );
    }

    void F_GPUData::_RegisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet)
    {
        _CriticalSection(
            [this, &InstanceSet]
            {
                ABYTEK_ENGINE_NFC_ASSERT(_IsUpdatePhase.test(boost::memory_order_acquire)) << "Cannot register instance sets outside update phase";
                InstanceSet->_Index = static_cast<U32>(_InstanceSets.size());
                _InstanceSets.push_back(InstanceSet);
                
                _InstanceSetHeaders.push_back(InstanceSet->GetHeader());
            }
        );
    }
    void F_GPUData::_UnregisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet)
    {
        _CriticalSection(
            [this, &InstanceSet]
            {
                ABYTEK_ENGINE_NFC_ASSERT(_IsUpdatePhase.test(boost::memory_order_acquire)) << "Cannot unregister instance sets outside update phase";
                _InstanceSets.back()->_Index = InstanceSet->_Index;
                std::swap(
                    _InstanceSets[InstanceSet->_Index],
                    _InstanceSets.back()
                );
                std::swap(
                    _InstanceSetHeaders[InstanceSet->_Index],
                    _InstanceSetHeaders.back()
                );
                _InstanceSets.pop_back();
                _InstanceSetHeaders.pop_back();
            }
        );
    }

    void F_GPUData::_AddDirtyInstanceSet(const TS<F_GPUDataInstanceSet>& InstanceSet)
    {
        _CriticalSection(
            [this, &InstanceSet]
            {
                ABYTEK_ENGINE_NFC_ASSERT(_IsUpdatePhase.test(boost::memory_order_acquire)) << "Cannot add dirty instance sets outside update phase";
                _DirtyInstanceSets.insert(InstanceSet);
            }
        );
    }
    void F_GPUData::_FlushDirtyInstanceSets(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        for (const auto& InstanceSet : _DirtyInstanceSets)
        {
            if (const auto& Allocation = InstanceSet->GetAllocation())
            {
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

    void F_GPUData::_UpdateInstanceSetHeaderBuffer(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto Context = H_RHI::GetMainContext();
        
        B8 IsEmpty = _InstanceSetHeaders.empty();
        
        auto SizeInBytes = sizeof(F_GPUDataInstanceSetHeader) * Max<Sz>(_InstanceSetHeaders.size(), 1);
        
        void* CachedInstanceSetHeadersPtr = H_Frame::GetArena(E_FrameParamType::RENDER)->AllocateData(
            SizeInBytes
        );
        if (!IsEmpty)
        {
            memcpy(
                CachedInstanceSetHeadersPtr,
                _InstanceSetHeaders.data(),
                SizeInBytes
            );
        }
        
        F_RHIBufferBuildParams BufferBuildParams;
        BufferBuildParams.Context = Context.Weak();
        BufferBuildParams.BufferAspect.SizeInBytes = SizeInBytes;
        BufferBuildParams.BufferAspect.StrideInBytes = sizeof(F_GPUDataInstanceSetHeader);
        if (!IsEmpty)
        {
            BufferBuildParams.BufferDataView = F_RHIBufferDataView(
                ((const U8*)CachedInstanceSetHeadersPtr),    
                ((const U8*)CachedInstanceSetHeadersPtr) + SizeInBytes    
            );
        }
        BufferBuildParams.AdditionalFlags |= E_RHIResourceAdditionalFlag::TRANSIENT;
        BufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeSRVCapabilities();
        _InstanceSetHeaderBuffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        _InstanceSetHeaderBuffer->SetDebugName(
            *GetDebugName()  
            + ABYTEK_TEXT(".InstanceSetHeaderBuffer")
        );
#endif
        
        F_RHIBufferViewBuildParams SRVBuildParams;
        SRVBuildParams.Context = Context.Weak();
        SRVBuildParams.BufferViewAspect.SizeInBytes = BufferBuildParams.BufferAspect.SizeInBytes;
        SRVBuildParams.BufferViewAspect.StrideInBytes = BufferBuildParams.BufferAspect.StrideInBytes;
        SRVBuildParams.Resource = _InstanceSetHeaderBuffer;
        SRVBuildParams.Access = F_RHIResourceAccess::MakeSRV();
        _InstanceSetHeaderSRV = RACreateAndBuildShared<A_RHIResourceView>(SRVBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        _InstanceSetHeaderSRV->SetDebugName(
            *GetDebugName()  
            + ABYTEK_TEXT(".InstanceSetHeaderSRV")
        );
#endif
        
        const auto& RHIFeatureSupports = GetRHIFeatureSupports();
        _InstanceSetHeaderBindGroup = _InstanceSetHeaderBinding.CreateBindGroup();
#ifdef ABYTEK_DEBUG_INFO
        _InstanceSetHeaderBindGroup->SetDebugName(
            *GetDebugName()  
            + ABYTEK_TEXT(".InstanceSetHeaderBindGroup")
        );
#endif
        _InstanceSetHeaderBindGroup->BindResourceView(
            GetBindGroupSlotName_InstanceSetHeaderBuffer(
                _Name, 
                RHIFeatureSupports
            ), 
            _InstanceSetHeaderSRV
        );
        {
            GPUData::F_InstanceSetHeadersUniformData UniformData;
            UniformData.Num = static_cast<U32>(_InstanceSetHeaders.size());
            _InstanceSetHeaderBindGroup->BindUniformData(
                GetBindGroupSlotName_InstanceSetHeadersUniformData(
                    _Name, 
                    RHIFeatureSupports
                ), 
                UniformData
            );
        }
        _InstanceSetHeaderBindGroup->Commit();
    }
}
