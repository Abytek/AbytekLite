#include "Abytek/GPUData/Render/GPUData.hpp"
#include "Abytek/GPUData/Render/GPUDataComponentStorage.hpp"
#include "Abytek/GPUData/Render/GPUDataInstanceSet.hpp"
#include "Abytek/GPUData/Render/GPUDataComponentType.hpp"
#include "Abytek/GPUData/Render/GPUDataIndexSet.hpp"
#include "Abytek/GPUData/Render/GlobalGPUDataPackProxy.hpp"
#include "Abytek/RenderBase/RenderScene.hpp"


namespace Abytek
{
    F_GPUData::F_GPUData(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_GPUDataBuildParams& BuildParams) :
        A_RenderObject(WorldRenderResource),
        _Scene(BuildParams.Scene)
    {
        {
            auto GlobalGPUDataPackProxy = WorldRenderResource->GetGlobalGPUDataPackProxy();
            for (const auto& CreateFunction : GlobalGPUDataPackProxy->GetComponentTypeCreateFunctions())
            {
                TS<F_GPUDataComponentType> ComponentType;
                F_GeneralTypeHashCode HashCode = 0;
                CreateFunction(ABYTEK_WTHIS(), ComponentType, HashCode);
                _GlobalComponentTypes.insert({ HashCode, ComponentType });
            }
        }
    }
    F_GPUData::~F_GPUData()
    {
        _GlobalComponentTypes = {};
    }

    void F_GPUData::BeginUpdate()
    {
        _CriticalSection(
            [this]
            {
                for (const auto& ComponentType : _ComponentTypes)
                {
                    ComponentType.second->GetStorage()->BeginUpdate();
                }
            }
        );
    }
    void F_GPUData::EndUpdate()
    {
        _CriticalSection(
            [this]
            {
                for (const auto& ComponentType : _ComponentTypes)
                {
                    ComponentType.second->GetStorage()->EndUpdate();
                }
            }
        );
    }
    void F_GPUData::BeginPostUpdate()
    {
        _CriticalSection(
            [this]
            {
                for (const auto& ComponentType : _ComponentTypes)
                {
                    ComponentType.second->GetStorage()->BeginPostUpdate();
                }
            }
        );
    }
    void F_GPUData::EndPostUpdate()
    {
        _CriticalSection(
            [this]
            {
                for (const auto& ComponentType : _ComponentTypes)
                {
                    ComponentType.second->GetStorage()->EndPostUpdate();
                }
            }
        );
    }

    void F_GPUData::_RegisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet)
    {
        _CriticalSection(
            [this, &InstanceSet]
            {
                _InstanceSets.insert(InstanceSet);
                for (const auto& ComponentType : InstanceSet->GetComponentTypes())
                {
                    auto ComponentTypeHashCode = ComponentType->GetHashCode();
                    for (
                        auto It = _ComponentTypeHashCodeToIndexSet.find(ComponentTypeHashCode);
                        It != _ComponentTypeHashCodeToIndexSet.upper_bound(ComponentTypeHashCode);
                        ++It
                    )
                    {
                        auto IndexSet = It->second;
                        if (IndexSet->HasInstanceSet(InstanceSet))
                        {
                            continue;
                        }
                        IndexSet->AddInstanceSet(InstanceSet);
                    }
                    ComponentType->AddInstanceSet(InstanceSet);
                }
            }
        );
    }
    void F_GPUData::_UnregisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet)
    {
        _CriticalSection(
            [this, &InstanceSet]
            {
                for (const auto& ComponentType : InstanceSet->GetComponentTypes())
                {
                    auto ComponentTypeHashCode = ComponentType->GetHashCode();
                    for (
                        auto It = _ComponentTypeHashCodeToIndexSet.find(ComponentTypeHashCode);
                        It != _ComponentTypeHashCodeToIndexSet.upper_bound(ComponentTypeHashCode);
                        ++It
                    )
                    {
                        auto IndexSet = It->second;
                        if (!IndexSet->HasInstanceSet(InstanceSet))
                        {
                            continue;
                        }
                        IndexSet->RemoveInstanceSet(InstanceSet);
                    }
                    ComponentType->RemoveInstanceSet(InstanceSet);
                }
                _InstanceSets.erase(_InstanceSets.find(InstanceSet));
            }
        );
    }

    void F_GPUData::_RegisterComponentType(const TW_Valid<F_GPUDataComponentType>& ComponentType)
    {
        _CriticalSection(
            [this, &ComponentType]
            {
                auto ComponentTypeHashCode = ComponentType->GetHashCode();
                _ComponentTypes[ComponentTypeHashCode] = ComponentType;
            }
        );
    }
    void F_GPUData::_UnregisterComponentType(const TW_Valid<F_GPUDataComponentType>& ComponentType)
    {
        _CriticalSection(
            [this, &ComponentType]
            {
                auto ComponentTypeHashCode = ComponentType->GetHashCode();
                ABYTEK_ENGINE_NFC_ASSERT(
                    _ComponentTypeHashCodeToIndexSet.find(ComponentTypeHashCode)
                    != _ComponentTypeHashCodeToIndexSet.end()
                ) << "Cannot unregister component types before unregistering all the related index sets";
                _ComponentTypes.erase(_ComponentTypes.find(ComponentTypeHashCode));
            }
        );
    }

    void F_GPUData::_RegisterIndexSet(const TW_Valid<F_GPUDataIndexSet>& IndexSet)
    {
        _CriticalSection(
            [this, &IndexSet]
            {
                for (const auto& ComponentType : IndexSet->GetComponentTypes())
                {
                    auto ComponentTypeHashCode = ComponentType->GetHashCode();
                    _ComponentTypeHashCodeToIndexSet.insert({
                        ComponentTypeHashCode,
                        IndexSet
                    });
                    for (const auto& InstanceSet : ComponentType->GetInstanceSets())
                    {
                        if (IndexSet->HasInstanceSet(InstanceSet))
                        {
                            continue;
                        }
                        IndexSet->AddInstanceSet(InstanceSet);
                    }
                }
            }
        );
    }
    void F_GPUData::_UnregisterIndexSet(const TW_Valid<F_GPUDataIndexSet>& IndexSet)
    {
        _CriticalSection(
            [this, &IndexSet]
            {
                for (const auto& ComponentType : IndexSet->GetComponentTypes())
                {
                    auto ComponentTypeHashCode = ComponentType->GetHashCode();
                    for (
                        auto It = _ComponentTypeHashCodeToIndexSet.find(ComponentTypeHashCode);
                        It != _ComponentTypeHashCodeToIndexSet.upper_bound(ComponentTypeHashCode);
                        ++It
                    )
                    {
                        if (It->second == IndexSet)
                        {
                            _ComponentTypeHashCodeToIndexSet.erase(It);
                            break;
                        }
                    }
                }
            }
        );
    }

    B8 F_GPUData::HasComponentType(F_GeneralTypeHashCode HashCode) const
    {
        return _ComponentTypes.find(HashCode) != _ComponentTypes.end();
    }
    TW<F_GPUDataComponentType> F_GPUData::FindComponentType(F_GeneralTypeHashCode HashCode) const
    {
        auto It = _ComponentTypes.find(HashCode);
        if (It == _ComponentTypes.end())
        {
            return {};
        }
        return It->second;
    }
    TW_Valid<F_GPUDataComponentType> F_GPUData::GetComponentType(F_GeneralTypeHashCode HashCode) const
    {
        auto It = _ComponentTypes.find(HashCode);
        ABYTEK_ENGINE_NFC_ASSERT(It == _ComponentTypes.end()) << "Not found component type with hash code: " << HashCode;
        return It->second;
    }

    B8 F_GPUData::HasGlobalComponentType(F_GeneralTypeHashCode HashCode) const
    {
        return _GlobalComponentTypes.find(HashCode) != _GlobalComponentTypes.end();
    }
    TS<F_GPUDataComponentType> F_GPUData::FindGlobalComponentType(F_GeneralTypeHashCode HashCode) const
    {
        auto It = _GlobalComponentTypes.find(HashCode);
        if (It == _GlobalComponentTypes.end())
        {
            return {};
        }
        return It->second;
    }
    TS<F_GPUDataComponentType> F_GPUData::GetGlobalComponentType(F_GeneralTypeHashCode HashCode) const
    {
        auto It = _GlobalComponentTypes.find(HashCode);
        ABYTEK_ENGINE_NFC_ASSERT(It == _GlobalComponentTypes.end()) << "Not found global component type with hash code: " << HashCode;
        return It->second;
    }
}
