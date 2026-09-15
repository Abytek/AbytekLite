#include "Abytek/GPUData/Render/GPUDataComponentType.hpp"
#include "Abytek/GPUData/Render/GPUDataComponentStorage.hpp"
#include "Abytek/GPUData/Render/GPUDataInstanceSet.hpp"
#include "Abytek/GPUData/Render/GPUData.hpp"


namespace Abytek
{
    F_GPUDataComponentType::F_GPUDataComponentType(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_GPUDataComponentTypeBuildParams& BuildParams) :
        A_RenderObject(WorldRenderResource),
        _GPUData(BuildParams.GPUData), 
        _HashCode(BuildParams.HashCode),
        _SizeInBytes(
            static_cast<U32>(
                AlignSize(BuildParams.SizeInBytes, BuildParams.AlignmentInBytes)
            )
        ),
        _AlignmentInBytes(BuildParams.AlignmentInBytes)
    { 
        F_GPUDataComponentStorageBuildParams StorageBuildParams;
        StorageBuildParams.ComponentType = ABYTEK_WTHIS();
        _Storage = TS<F_GPUDataComponentStorage>()(WorldRenderResource, StorageBuildParams);
        _GPUData->_RegisterComponentType(ABYTEK_WTHIS());
    }
    F_GPUDataComponentType::~F_GPUDataComponentType()
    {
        _GPUData->_UnregisterComponentType(ABYTEK_WTHIS()); 
        _Storage = {};
    }

    B8 F_GPUDataComponentType::HasInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet) const
    {
        return _InstanceSets.find(InstanceSet) != _InstanceSets.end();
    }
    void F_GPUDataComponentType::AddInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet)
    {
        _InstanceSets.insert(InstanceSet);
    }
    void F_GPUDataComponentType::RemoveInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet)
    {
        _InstanceSets.erase(_InstanceSets.find(InstanceSet));
    }

    TF_Optional<F_GPUDataComponentAllocation> F_GPUDataComponentType::New(U32 NumComponents)
    {
        return _Storage->Allocate(NumComponents);
    }
    void F_GPUDataComponentType::Delete(const F_GPUDataComponentAllocation& ComponentAllocation)
    {
        _Storage->DeallocationQueue.Push(ComponentAllocation);
    }
}
