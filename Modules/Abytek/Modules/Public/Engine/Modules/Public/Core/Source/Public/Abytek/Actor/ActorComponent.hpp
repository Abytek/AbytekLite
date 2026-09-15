#pragma once

#include "Abytek/World/WorldContext.hpp"
#include "Abytek/Cookable.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API A_ActorComponent : public A_WorldContext, public I_Cookable
    {
    public:
        friend class F_Actor;
        friend class F_ActorComponentManager;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(A_ActorComponent)
        
    public:
        static F_Name GetTickUpdateFunctionName();
        static F_Name GetPostTickUpdateFunctionName();
        static void GlobalInit();
        static void GlobalRelease();
    
    private:
        TW<F_Actor> _Actor;
        B8 _IsRegistered = false;
        B8 _BeganPlay = false;
        TF_Vector<F_Name> _Tags;
        B8 _IsEnabledTick = false;
        B8 _IsEnabledPostTick = false;
        TF_Vector<TS<A_ActorComponent>> _ChildInstanceComponents;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetActor() const noexcept
        {
            return _Actor;
        }
        ABYTEK_FORCE_INLINE auto IsRegistered() const noexcept
        {
            return _IsRegistered;
        }
        ABYTEK_FORCE_INLINE auto BeganPlay() const noexcept
        {
            return _BeganPlay;
        }
        ABYTEK_FORCE_INLINE B8 HasTag(const F_Name& Tag) const noexcept
        {
            return (
                std::find(
                    _Tags.begin(),    
                    _Tags.end(),
                    Tag
                )    
                != _Tags.end()
            );
        }
        ABYTEK_FORCE_INLINE void AddTag(const F_Name& Tag) noexcept
        {
            ABYTEK_ENGINE_NFC_ASSERT(!HasTag(Tag)) << "Already added tag \"" << Tag << "\" to actor component \"" << GetName() << "\"";
            _Tags.push_back(Tag);
        }
        ABYTEK_FORCE_INLINE void RemoveTag(const F_Name& Tag) noexcept
        {
            ABYTEK_ENGINE_NFC_ASSERT(HasTag(Tag)) << "Not found tag \"" << Tag << "\" in actor component \"" << GetName() << "\"";
            _Tags.erase(
                std::find(
                    _Tags.begin(),    
                    _Tags.end(),
                    Tag
                )   
            );
        }
        ABYTEK_FORCE_INLINE void EnsureTag(const F_Name& Tag) noexcept
        {
            if (HasTag(Tag))
            {
                return;
            }
            AddTag(Tag);
        }
        ABYTEK_FORCE_INLINE auto IsEnabledTick() const noexcept
        {
            return _IsEnabledTick;
        }
        ABYTEK_FORCE_INLINE auto IsEnabledPostTick() const noexcept
        {
            return _IsEnabledPostTick;
        }
        ABYTEK_FORCE_INLINE const auto& GetChildInstanceComponents() const noexcept
        {
            return _ChildInstanceComponents;
        }
        
    protected:
        A_ActorComponent(const F_SerializableObjectInitParams& InitParams);
        
    public:
        ~A_ActorComponent() override;
        
    protected:
        virtual void OnAddToActor();
        virtual void OnRemoveFromActor();
        virtual void OnRegisterComponent();
        virtual void OnUnregisterComponent();
        virtual void OnBeginPlayComponent();
        virtual void OnEndPlayComponent();
        virtual void OnTick();
        virtual void OnPostTick();
        
    public:
        void RegisterComponent();
        void UnregisterComponent();
        void BeginPlayComponent();
        void EndPlayComponent();
        
    public:
        void MakeRuntimeComponentReady();
        
    public:
        void EnableTick();
        void DisableTick();
        void EnablePostTick();
        void DisablePostTick();
        
    private:
        void _ActualEnableTick();
        void _ActualDisableTick();
        void _ActualEnablePostTick();
        void _ActualDisablePostTick();
        
    public:
        void AddChildInstanceComponent(const TS<A_ActorComponent>& Component);
        
    private:
        void _AddToActor(const TW_Valid<F_Actor>& Actor);
        void _RemoveFromActor();
    };
}