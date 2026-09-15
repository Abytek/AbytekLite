#include "Abytek/Actor/Actor.hpp"
#include "Abytek/Actor/ActorComponent.hpp"
#include "Abytek/Level/Level.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/World/WorldContextHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_Actor)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Actor"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Level);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_SerializedOwnedComponents);
    }
    
    F_Actor::F_Actor(const F_SerializableObjectInitParams& InitParams) :
        A_WorldContext(InitParams)
    {
    }
    F_Actor::~F_Actor()
    {
    }

    void F_Actor::OnLoad()
    {
        auto SerializedOwnedComponents = ABYTEK_MOVE(_SerializedOwnedComponents);
        for (const auto& Component : SerializedOwnedComponents)
        {
            AddOwnedComponent(Component);
        }
    }
    void F_Actor::OnUnload()
    {
        while (_OwnedComponents.size() > 0)
        {
            RemoveOwnedComponent(_OwnedComponents.back().Weak());
        }
        ABYTEK_ENGINE_CORE_ASSERT(_OwnedComponents.size() == 0);
        ABYTEK_ENGINE_CORE_ASSERT(_InstanceComponents.size() == 0);
    }

    void F_Actor::OnPrepareForSaving(const TW_Valid<F_SerializableEnvironment>& Environment)
    {
        _SerializedOwnedComponents = {};
        for (const auto& Component : _OwnedComponents)
        {
            if (Component->CanSerialize())
            {
                _SerializedOwnedComponents.push_back(Component);
            }
        }
    }
    void F_Actor::OnCleanUpAfterSaving(const TW_Valid<F_SerializableEnvironment>& Environment)
    {
        _SerializedOwnedComponents = {};
    }

    B8 F_Actor::HasOwnedComponent(const TW_Valid<A_ActorComponent>& Component) const
    {
        auto SComponent = ShareObject(Component);
        return (
            std::find(
                _OwnedComponents.begin(),
                _OwnedComponents.end(),
                SComponent
            )
            != _OwnedComponents.end()
        );
    }
    TW<A_ActorComponent> F_Actor::FindOwnedComponent(const F_Name& ComponentName) const
    {
        for (const auto& Component : _OwnedComponents)
        {
            if (Component->GetName() == ComponentName)
            {
                return Component.Weak();
            }
        }
        return {};
    }
    TW<A_ActorComponent> F_Actor::FindOwnedComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const
    {
        for (const auto& Component : _OwnedComponents)
        {
            if (Component->GetType()->SupportImplicitPolymorphismCast(ComponentType))
            {
                return Component.Weak();
            }
        }
        return {};
    }
    TW_Valid<A_ActorComponent> F_Actor::GetOwnedComponent(const F_Name& ComponentName) const
    {
        auto Component = FindOwnedComponent(ComponentName);
        ABYTEK_ENGINE_NFC_ASSERT(Component) << "Not found component with name: " << ComponentName;
        return Component;
    }
    TW_Valid<A_ActorComponent> F_Actor::GetOwnedComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const
    {
        auto Component = FindOwnedComponent(ComponentType);
        ABYTEK_ENGINE_NFC_ASSERT(Component) << "Not found component with type: " << ComponentType->GetCanonical();
        return Component;
    }
    void F_Actor::FindOwnedComponents(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType, TF_Vector<TW<A_ActorComponent>>& OutComponents) const
    {
        for (const auto& Component : _OwnedComponents)
        {
            if (Component->GetType()->SupportImplicitPolymorphismCast(ComponentType))
            {
                OutComponents.push_back(Component.Weak());
            }
        }
    }
    TW_Valid<A_ActorComponent> F_Actor::AddOwnedComponent(const TS<A_ActorComponent>& Component)
    {
        ABYTEK_ENGINE_NFC_ASSERT(!HasOwnedComponent(Component.Weak())) << "Already added component";
        ABYTEK_ENGINE_NFC_ASSERT(Component->GetEnvironment() == GetEnvironment()) << "Environment mismatch";
        Component->_Actor = ABYTEK_WTHIS();
        Component->_AddToActor(ABYTEK_WTHIS());
        _OwnedComponents.push_back(Component);
        return Component.Weak();
    }
    TW_Valid<A_ActorComponent> F_Actor::AddOwnedComponent(const F_Name& ComponentName, const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType)
    {
        auto Component = H_WorldContext::CreateObject<A_ActorComponent>(
            ABYTEK_WTHIS(),
            ComponentName,
            GetPackageName(),
            ComponentType
        );
        AddOwnedComponent(Component);
        return Component.Weak();
    }
    TS<A_ActorComponent> F_Actor::RemoveOwnedComponent(const TW_Valid<A_ActorComponent>& Component)
    {
        ABYTEK_ENGINE_NFC_ASSERT(HasOwnedComponent(Component.Weak())) << "Not found component";
        auto SComponent = ShareObject(Component);
        _OwnedComponents.erase(
            std::find(
                _OwnedComponents.begin(),
                _OwnedComponents.end(),
                SComponent
            )  
        );
        Component->_RemoveFromActor();
        Component->_Actor = {};
        return SComponent;
    }

    B8 F_Actor::HasInstanceComponent(const TW_Valid<A_ActorComponent>& Component) const
    {
        auto SComponent = ShareObject(Component);
        return (
            std::find(
                _InstanceComponents.begin(),
                _InstanceComponents.end(),
                SComponent
            )
            != _InstanceComponents.end()
        );
    }
    TW<A_ActorComponent> F_Actor::FindInstanceComponent(const F_Name& ComponentName) const
    {
        for (const auto& Component : _InstanceComponents)
        {
            if (Component->GetName() == ComponentName)
            {
                return Component.Weak();
            }
        }
        return {};
    }
    TW<A_ActorComponent> F_Actor::FindInstanceComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const
    {
        for (const auto& Component : _InstanceComponents)
        {
            if (Component->GetType()->SupportImplicitPolymorphismCast(ComponentType))
            {
                return Component.Weak();
            }
        }
        return {};
    }
    TW_Valid<A_ActorComponent> F_Actor::GetInstanceComponent(const F_Name& ComponentName) const
    {
        auto Component = FindInstanceComponent(ComponentName);
        ABYTEK_ENGINE_NFC_ASSERT(Component) << "Not found component with name: " << ComponentName;
        return Component;
    }
    TW_Valid<A_ActorComponent> F_Actor::GetInstanceComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const
    {
        auto Component = FindInstanceComponent(ComponentType);
        ABYTEK_ENGINE_NFC_ASSERT(Component) << "Not found component with type: " << ComponentType->GetCanonical();
        return Component;
    }
    void F_Actor::FindInstanceComponents(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType, TF_Vector<TW<A_ActorComponent>>& OutComponents) const
    {
        for (const auto& Component : _InstanceComponents)
        {
            if (Component->GetType()->SupportImplicitPolymorphismCast(ComponentType))
            {
                OutComponents.push_back(Component.Weak());
            }
        }
    }
    TW_Valid<A_ActorComponent> F_Actor::AddInstanceComponent(const TS<A_ActorComponent>& Component)
    {
        ABYTEK_ENGINE_NFC_ASSERT(!HasInstanceComponent(Component.Weak())) << "Already added component";
        ABYTEK_ENGINE_NFC_ASSERT(Component->GetEnvironment() == GetEnvironment()) << "Environment mismatch";
        Component->_Actor = ABYTEK_WTHIS();
        Component->_AddToActor(ABYTEK_WTHIS());
        _InstanceComponents.push_back(Component);
        return Component.Weak();
    }
    TW_Valid<A_ActorComponent> F_Actor::AddInstanceComponent(const F_Name& ComponentName, const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType)
    {
        auto Component = H_WorldContext::CreateObject<A_ActorComponent>(
            ABYTEK_WTHIS(),
            ComponentName,
            GetPackageName(),
            ComponentType
        );
        AddInstanceComponent(Component);
        return Component.Weak();
    }
    TS<A_ActorComponent> F_Actor::RemoveInstanceComponent(const TW_Valid<A_ActorComponent>& Component)
    {
        ABYTEK_ENGINE_NFC_ASSERT(HasInstanceComponent(Component.Weak())) << "Not found component";
        auto SComponent = ShareObject(Component);
        _InstanceComponents.erase(
            std::find(
                _InstanceComponents.begin(),
                _InstanceComponents.end(),
                SComponent
            )  
        );
        Component->_RemoveFromActor();
        Component->_Actor = {};
        return SComponent;
    }

    B8 F_Actor::HasComponent(const TW_Valid<A_ActorComponent>& Component) const
    {
        if (HasOwnedComponent(Component))
        {
            return true;
        }
        if (HasInstanceComponent(Component))
        {
            return true;
        }
        return false;
    }
    TW<A_ActorComponent> F_Actor::FindComponent(const F_Name& ComponentName) const
    {
        if (auto Component = FindOwnedComponent(ComponentName))
        {
            return Component;
        }
        if (auto Component = FindInstanceComponent(ComponentName))
        {
            return Component;
        }
        return {};
    }
    TW<A_ActorComponent> F_Actor::FindComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const
    {
        if (auto Component = FindOwnedComponent(ComponentType))
        {
            return Component;
        }
        if (auto Component = FindInstanceComponent(ComponentType))
        {
            return Component;
        }
        return {};
    }
    TW_Valid<A_ActorComponent> F_Actor::GetComponent(const F_Name& ComponentName) const
    {
        auto Component = FindComponent(ComponentName);
        ABYTEK_ENGINE_NFC_ASSERT(Component) << "Not found component with name: " << ComponentName;
        return Component;
    }
    TW_Valid<A_ActorComponent> F_Actor::GetComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const
    {
        auto Component = FindComponent(ComponentType);
        ABYTEK_ENGINE_NFC_ASSERT(Component) << "Not found component with type: " << ComponentType->GetCanonical();
        return Component;
    }

    void F_Actor::RegisterComponents()
    {
        ABYTEK_ENGINE_NFC_ASSERT(GetLevel());
        for (const auto& Component : _OwnedComponents)
        {
            if (!Component->IsRegistered())
            {
                Component->RegisterComponent();
            }
        }
    }
    void F_Actor::UnregisterComponents()
    {
        ABYTEK_ENGINE_NFC_ASSERT(GetLevel());
        for (auto It = _OwnedComponents.rbegin(); It != _OwnedComponents.rend(); ++It)
        {
            const auto& Component = *It;
            if (Component->IsRegistered())
            {
                Component->UnregisterComponent();
            }
        }
    }

    void F_Actor::BeginPlayComponents()
    {
        ABYTEK_ENGINE_NFC_ASSERT(GetLevel());
        for (const auto& Component : _OwnedComponents)
        {
            if (!Component->BeganPlay())
            {
                Component->BeginPlayComponent();
            }
        }
    }
    void F_Actor::EndPlayComponents()
    {
        ABYTEK_ENGINE_NFC_ASSERT(GetLevel());
        for (auto It = _OwnedComponents.rbegin(); It != _OwnedComponents.rend(); ++It)
        {
            const auto& Component = *It;
            if (Component->BeganPlay())
            {
                Component->EndPlayComponent();
            }
        }
    }

    void F_Actor::MakeRuntimeComponentsReady()
    {
        ABYTEK_ENGINE_NFC_ASSERT(GetLevel());
        for (const auto& Component : _OwnedComponents)
        {
            Component->MakeRuntimeComponentReady();
        }
    }
}
