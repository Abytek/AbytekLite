#include "Abytek/GPUData/Render/GPUDataInstanceSet.hpp"
#include "Abytek/GPUData/Render/GPUDataComponentStorage.hpp"
#include "Abytek/GPUData/Render/GPUData.hpp"


namespace Abytek
{
    F_GPUDataInstanceSet::F_GPUDataInstanceSet(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_GPUDataInstanceSetBuildParams& BuildParams) :
        A_RenderObject(WorldRenderResource),
        _GPUData(BuildParams.GPUData),
        _ComponentTypes(BuildParams.ComponentTypes),
        _Num(BuildParams.Num)
    {
        _GPUData->_RegisterInstanceSet(ABYTEK_WTHIS());
        _AllocateComponents();
    }
    F_GPUDataInstanceSet::~F_GPUDataInstanceSet()
    {
        _DeallocateComponents();
        _GPUData->_UnregisterInstanceSet(ABYTEK_WTHIS());
    }

    void F_GPUDataInstanceSet::_AllocateComponents()
    {
        _IsUsable = true;
        U32 NumComponentTypes = static_cast<U32>(_ComponentTypes.size());
        _ComponentAllocations.reserve(NumComponentTypes);
        _ComponentAllocations.clear();
        for (U32 Idx = 0; Idx < NumComponentTypes; ++Idx)
        {
            const auto& ComponentType = _ComponentTypes[Idx];
            if (auto ComponentAllocation = ComponentType->New(_Num))
            {
                _ComponentAllocations.push_back(*ComponentAllocation);
                continue;
            }
            _IsUsable = false;
            break;
        }
        if (!_IsUsable)
        {
            _DeallocateComponents();
        }
    }
    void F_GPUDataInstanceSet::_DeallocateComponents()
    {
        U32 NumComponentTypes = static_cast<U32>(_ComponentTypes.size());
        for (U32 Idx = 0; Idx < NumComponentTypes; ++Idx)
        {
            const auto& ComponentType = _ComponentTypes[Idx];
            const auto& ComponentAllocation = _ComponentAllocations[Idx];
            ComponentType->Delete(ComponentAllocation);
        }
        _ComponentAllocations = {};
    }

    B8 F_GPUDataInstanceSet::HasComponentType(const TW_Valid<F_GPUDataComponentType>& ComponentType) const
    {
        for (const auto& X : _ComponentTypes)
        {
            if (X.Weak() == ComponentType)
            {
                return true;
            }
        }
        return false;
    }
    U32 F_GPUDataInstanceSet::GetComponentTypeIndex(const TW_Valid<F_GPUDataComponentType>& ComponentType) const
    {
        U32 NumComponentTypes = _ComponentTypes.size();
        for (U32 Idx = 0; Idx < NumComponentTypes; ++Idx)
        {
            if (_ComponentTypes[Idx].Weak() == ComponentType)
            {
                return Idx;
            }
        }
        ABYTEK_LOG_FATAL() << "Not found component type in instance set: " << ComponentType->GetHashCode();
        return ~U32(0);
    }

    void F_GPUDataInstanceSet::UploadComponent(U32 Index, const void* DataPtr)
    {
    }
    void F_GPUDataInstanceSet::UploadComponent(const TW_Valid<F_GPUDataComponentType>& ComponentType, const void* DataPtr)
    {
        UploadComponent(GetComponentTypeIndex(ComponentType), DataPtr);
    }
}
