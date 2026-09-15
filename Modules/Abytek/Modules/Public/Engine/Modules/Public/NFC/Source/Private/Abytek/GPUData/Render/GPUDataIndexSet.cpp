#include "Abytek/GPUData/Render/GPUDataIndexSet.hpp"
#include "Abytek/GPUData/Render/GPUDataInstanceSet.hpp"
#include "Abytek/GPUData/Render/GPUDataComponentType.hpp"
#include "Abytek/GPUData/Render/GPUData.hpp"


namespace Abytek
{
    F_GPUDataIndexSet::F_GPUDataIndexSet(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_GPUDataIndexSetBuildParams& BuildParams) :
        A_RenderObject(WorldRenderResource),
        _GPUData(BuildParams.GPUData)
    {
        _ComponentTypes.reserve(BuildParams.ComponentTypes.size());
        for (const auto& ComponentType : BuildParams.ComponentTypes)
        {
            ABYTEK_ENGINE_NFC_ASSERT(ComponentType) << "Invalid component type";
            ABYTEK_ENGINE_NFC_ASSERT(
                std::find(
                    _ComponentTypes.begin(),
                    _ComponentTypes.end(),
                    ComponentType
                )  
                == _ComponentTypes.end()
            ) << "Duplicated component type: " << ComponentType->GetHashCode();
            _StrideInBytes = AlignAddress(_StrideInBytes, ComponentType->GetAlignmentInBytes());
            _StrideInBytes += ComponentType->GetSizeInBytes();
            _ComponentTypes.push_back(ComponentType);
        }
        _StrideInBytes = AlignSize(_StrideInBytes, 16);
        _GPUData->_RegisterIndexSet(ABYTEK_WTHIS());
    }
    F_GPUDataIndexSet::~F_GPUDataIndexSet()
    {
        _GPUData->_UnregisterIndexSet(ABYTEK_WTHIS());
        _ComponentTypes = {};
    }

    B8 F_GPUDataIndexSet::HasInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet) const
    {
        return _InstanceSetToIndex.find(InstanceSet) != _InstanceSetToIndex.end();
    }
    U32 F_GPUDataIndexSet::GetInstanceSetIndex(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet) const
    {
        auto It = _InstanceSetToIndex.find(InstanceSet);
        ABYTEK_ENGINE_NFC_ASSERT(It != _InstanceSetToIndex.end());
        return It->second;
    }
    TW_Valid<F_GPUDataInstanceSet> F_GPUDataIndexSet::GetInstanceSetAtIndex(U32 Index) const
    {
        return _InstanceSets[Index];
    }
    void F_GPUDataIndexSet::AddInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet)
    {
        U32 InstanceSetIndex = static_cast<U32>(_InstanceSets.size());
        _InstanceSets.push_back(InstanceSet);
        _DirtyInstanceSets.insert(InstanceSet);
        _InstanceSetToIndex[InstanceSet] = InstanceSetIndex;
    }
    void F_GPUDataIndexSet::RemoveInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet)
    {
        auto It = _InstanceSetToIndex.find(InstanceSet);
        U32 InstanceSetIndex = It->second;
        if (const auto& NewestInstanceSet = _InstanceSets.back(); NewestInstanceSet != InstanceSet)
        {
            auto NewestIt = _InstanceSetToIndex.find(NewestInstanceSet);
            NewestIt->second = InstanceSetIndex;
            _InstanceSets[InstanceSetIndex] = NewestInstanceSet;
            if (_DirtyInstanceSets.find(NewestInstanceSet) != _DirtyInstanceSets.end())
            {
                _DirtyInstanceSets.insert(NewestInstanceSet);
            }
        }
        _InstanceSetToIndex.erase(It);
        if (auto DirtyIt = _DirtyInstanceSets.find(InstanceSet); DirtyIt != _DirtyInstanceSets.end())
        {
            _DirtyInstanceSets.erase(DirtyIt);
        }
        _InstanceSets.pop_back();
    }

    void F_GPUDataIndexSet::_UpdateBufferSize()
    {
        U32 MinBufferSize = static_cast<U32>(_InstanceSets.size());
        U32 LastBufferSize = _LastBufferSize;
        U32 NewBufferSize = LastBufferSize;
        
        if ((LastBufferSize / 4) > MinBufferSize)
        {
            while ((NewBufferSize / 4) > MinBufferSize)
            {
                NewBufferSize /= 2;
            }
        }
        else if (LastBufferSize > MinBufferSize)
        {
            return;
        }
        else
        {
            while (LastBufferSize > NewBufferSize)
            {
                NewBufferSize = Max<U32>(NewBufferSize * 2, 1);
            }
        }
        
        TS<A_RHIResource> Buffer;
        if (NewBufferSize)
        {
            F_RHIBufferBuildParams BufferBuildParams;
            BufferBuildParams.Context = H_RHI::GetMainContext().Weak();
            BufferBuildParams.BufferAspect.SizeInBytes = static_cast<U64>(NewBufferSize * _StrideInBytes);
            BufferBuildParams.BufferAspect.StrideInBytes = _StrideInBytes;
            BufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeSRVCapabilities();
            Buffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
        
            if (_LastBufferSize)
            {
                H_RHIPassUtilities::CopyBuffer(
                    *H_RHI::GetMainProcess(),
                    Buffer,
                    0,
                    _Buffer,
                    0,
                    Min<U32>(NewBufferSize, _LastBufferSize)
                );
            }
        }
        
        _Buffer = Buffer;
        _LastBufferSize = NewBufferSize;
    }
    void F_GPUDataIndexSet::_UploadDirtyInstanceSets()
    {
        if (_DirtyInstanceSets.size() == 0)
        {
            return;
        }
        
        // Build upload ranges
        TF_SmallVector<U32, 32> Indices;
        for (const auto& InstanceSet : _DirtyInstanceSets)
        {
            Indices.push_back(_InstanceSetToIndex.find(InstanceSet)->second);
        }
        boost::sort(Indices);
        struct F_UploadRange
        {
            U32 BeginIndex = 0;
            U32 EndIndex = 0;
            
            U32 GetSize() const
            {
                return EndIndex - BeginIndex;
            }
        };
        F_UploadRange CurrentUploadRange;
        TF_SmallVector<F_UploadRange, 16> UploadRanges;
        auto FlushUploadRanges = [&]
        {
            if (CurrentUploadRange.GetSize() == 0)
            {
                return;
            }
            UploadRanges.push_back(CurrentUploadRange);
            CurrentUploadRange.BeginIndex = CurrentUploadRange.EndIndex;
        };
        for (U32 Index : Indices)
        {
            if (CurrentUploadRange.EndIndex < Index)
            {
                FlushUploadRanges();
                CurrentUploadRange.BeginIndex = Index;
            }
            CurrentUploadRange.EndIndex = Index + 1;
        }
        FlushUploadRanges();
        
        // Upload
        for (const auto& UploadRange : UploadRanges)
        {
            TF_SmallVector<F_GPUDataComponentAddress, 16> ComponentAddresses;
            for (U32 Index = UploadRange.BeginIndex; Index < UploadRange.EndIndex; ++Index)
            {
                const auto& InstanceSet = _InstanceSets[Index];
            }
        }
        
        _DirtyInstanceSets = {};
    }

    void F_GPUDataIndexSet::Update()
    {
        _UpdateBufferSize();
        _UploadDirtyInstanceSets();
    }
}
