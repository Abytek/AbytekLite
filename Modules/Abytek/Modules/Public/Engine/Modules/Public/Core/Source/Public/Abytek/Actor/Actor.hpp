#pragma once

#include "Abytek/World/WorldContext.hpp"
#include "Abytek/Cookable.hpp"
 

namespace Abytek
{
    class A_ActorComponent;
    class F_Level;

    class ABYTEK_ENGINE_CORE_API F_Actor : public A_WorldContext, public I_Cookable
    {
    public:
        friend class F_Level;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(F_Actor)
    
    private:
        TW<F_Level> _Level;
        TF_Vector<TS<A_ActorComponent>> _OwnedComponents;
        TF_Vector<TS<A_ActorComponent>> _InstanceComponents;
        TF_Vector<TS<A_ActorComponent>> _SerializedOwnedComponents;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetLevel() const noexcept
        {
            return _Level;
        }
        ABYTEK_FORCE_INLINE const auto& GetOwnedComponents() const noexcept
        {
            return _OwnedComponents;
        }
        ABYTEK_FORCE_INLINE const auto& GetInstanceComponents() const noexcept
        {
            return _InstanceComponents;
        }
        ABYTEK_FORCE_INLINE const auto& GetSerializedOwnedComponents() const noexcept
        {
            return _SerializedOwnedComponents;
        }
        
    public:
        F_Actor(const F_SerializableObjectInitParams& InitParams);
        ~F_Actor() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    protected:
        void OnPrepareForSaving(const TW_Valid<F_SerializableEnvironment>& Environment) override;
        void OnCleanUpAfterSaving(const TW_Valid<F_SerializableEnvironment>& Environment) override;
        
    public:
        B8 HasOwnedComponent(const TW_Valid<A_ActorComponent>& Component) const;
        TW<A_ActorComponent> FindOwnedComponent(const F_Name& ComponentName) const;
        TW<A_ActorComponent> FindOwnedComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const;
        template<typename __F_Component>
        TW<__F_Component> FindOwnedComponent(const TF_ReflectionTypeHandle<__F_Component>& ComponentType = TF_ReflectionTypeHandle<__F_Component>(F_ReflectionContext::GetGlobal())) const
        {
            if (auto Component = FindOwnedComponent(ComponentType.template Cast<A_ActorComponent>()))
            {
                return Component.template FastCast<__F_Component>();
            }
            return {};
        }
        TW_Valid<A_ActorComponent> GetOwnedComponent(const F_Name& ComponentName) const;
        TW_Valid<A_ActorComponent> GetOwnedComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const;
        template<typename __F_Component>
        TW_Valid<__F_Component> GetOwnedComponent(const TF_ReflectionTypeHandle<__F_Component>& ComponentType = TF_ReflectionTypeHandle<__F_Component>(F_ReflectionContext::GetGlobal())) const
        {
            return GetOwnedComponent(ComponentType.template Cast<A_ActorComponent>()).template FastCast<__F_Component>();
        }
        void FindOwnedComponents(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType, TF_Vector<TW<A_ActorComponent>>& OutComponents) const;
        TW_Valid<A_ActorComponent> AddOwnedComponent(const TS<A_ActorComponent>& Component);
        TW_Valid<A_ActorComponent> AddOwnedComponent(
            const F_Name& ComponentName,
            const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType
        );
        template<typename __F_Component>
        TW_Valid<__F_Component> AddOwnedComponent(
            const F_Name& ComponentName,
            const TF_ReflectionTypeHandle<__F_Component>& ComponentType = TF_ReflectionTypeHandle<__F_Component>(F_ReflectionContext::GetGlobal())
        )
        {
            return AddOwnedComponent(
                ComponentName, 
                ComponentType.template Cast<A_ActorComponent>()
            ).FastCast<__F_Component>();
        }
        TS<A_ActorComponent> RemoveOwnedComponent(const TW_Valid<A_ActorComponent>& Component);
        
    public:
        B8 HasInstanceComponent(const TW_Valid<A_ActorComponent>& Component) const;
        TW<A_ActorComponent> FindInstanceComponent(const F_Name& ComponentName) const;
        TW<A_ActorComponent> FindInstanceComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const;
        template<typename __F_Component>
        TW<__F_Component> FindInstanceComponent(const TF_ReflectionTypeHandle<__F_Component>& ComponentType = TF_ReflectionTypeHandle<__F_Component>(F_ReflectionContext::GetGlobal())) const
        {
            if (auto Component = FindInstanceComponent(ComponentType.template Cast<A_ActorComponent>()))
            {
                return Component.template FastCast<__F_Component>();
            }
            return {};
        }
        TW_Valid<A_ActorComponent> GetInstanceComponent(const F_Name& ComponentName) const;
        TW_Valid<A_ActorComponent> GetInstanceComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const;
        template<typename __F_Component>
        TW_Valid<__F_Component> GetInstanceComponent(const TF_ReflectionTypeHandle<__F_Component>& ComponentType = TF_ReflectionTypeHandle<__F_Component>(F_ReflectionContext::GetGlobal())) const
        {
            return GetInstanceComponent(ComponentType.template Cast<A_ActorComponent>()).template FastCast<__F_Component>();
        }
        void FindInstanceComponents(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType, TF_Vector<TW<A_ActorComponent>>& OutComponents) const;
        TW_Valid<A_ActorComponent> AddInstanceComponent(const TS<A_ActorComponent>& Component);
        TW_Valid<A_ActorComponent> AddInstanceComponent(
            const F_Name& ComponentName,
            const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType
        );
        template<typename __F_Component>
        TW_Valid<__F_Component> AddInstanceComponent(
            const F_Name& ComponentName,
            const TF_ReflectionTypeHandle<__F_Component>& ComponentType = TF_ReflectionTypeHandle<__F_Component>(F_ReflectionContext::GetGlobal())
        )
        {
            return AddInstanceComponent(
                ComponentName, 
                ComponentType.template Cast<A_ActorComponent>()
            ).FastCast<__F_Component>();
        }
        TS<A_ActorComponent> RemoveInstanceComponent(const TW_Valid<A_ActorComponent>& Component);
        
        
    public:
        B8 HasComponent(const TW_Valid<A_ActorComponent>& Component) const;
        TW<A_ActorComponent> FindComponent(const F_Name& ComponentName) const;
        TW<A_ActorComponent> FindComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const;
        template<typename __F_Component>
        TW<__F_Component> FindComponent(const TF_ReflectionTypeHandle<__F_Component>& ComponentType = TF_ReflectionTypeHandle<__F_Component>(F_ReflectionContext::GetGlobal())) const
        {
            if (auto Component = FindComponent(ComponentType.template Cast<A_ActorComponent>()))
            {
                return Component.template FastCast<__F_Component>();
            }
            return {};
        }
        TW_Valid<A_ActorComponent> GetComponent(const F_Name& ComponentName) const;
        TW_Valid<A_ActorComponent> GetComponent(const TF_ReflectionTypeHandle<A_ActorComponent>& ComponentType) const;
        template<typename __F_Component>
        TW_Valid<__F_Component> GetComponent(const TF_ReflectionTypeHandle<__F_Component>& ComponentType = TF_ReflectionTypeHandle<__F_Component>(F_ReflectionContext::GetGlobal())) const
        {
            return GetComponent(ComponentType.template Cast<A_ActorComponent>()).template FastCast<__F_Component>();
        }
        
    public:
        void RegisterComponents();
        void UnregisterComponents();
        
    public:
        void BeginPlayComponents();
        void EndPlayComponents();
        
    public:
        void MakeRuntimeComponentsReady();
    };
}