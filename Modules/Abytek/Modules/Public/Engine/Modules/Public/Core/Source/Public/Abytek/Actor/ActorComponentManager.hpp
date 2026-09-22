#pragma once

#include "Abytek/ApplicationSubsystem.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    class A_ActorComponent;

    namespace ActorComponentManagerEvents
    {
        using F_OnRegisterComponentToTick = TF_Event<void(const TW_Valid<A_ActorComponent>& Component)>;
        using F_OnUnregisterComponentFromTick = TF_Event<void(const TW_Valid<A_ActorComponent>& Component)>;
        using F_OnRegisterComponentToPostTick = TF_Event<void(const TW_Valid<A_ActorComponent>& Component)>;
        using F_OnUnregisterComponentFromPostTick = TF_Event<void(const TW_Valid<A_ActorComponent>& Component)>;
    }
    struct F_ActorComponentManagerEvents
    {
        ActorComponentManagerEvents::F_OnRegisterComponentToTick OnRegisterComponentToTick;
        ActorComponentManagerEvents::F_OnUnregisterComponentFromTick OnUnregisterComponentFromTick;
        ActorComponentManagerEvents::F_OnRegisterComponentToPostTick OnRegisterComponentToPostTick;
        ActorComponentManagerEvents::F_OnUnregisterComponentFromPostTick OnUnregisterComponentFromPostTick;
    };
    
    class ABYTEK_ENGINE_CORE_API F_ActorComponentManager final : public A_ApplicationSubsystem
    {
    public:
        friend class A_ActorComponent;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_ActorComponentManager);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_ActorComponentManager);

    private:
        TF_Set<TW<A_ActorComponent>> _ActorComponentsToTick;
        TF_Set<TW<A_ActorComponent>> _ActorComponentsToPostTick;
        
    public:
        F_ActorComponentManagerEvents Events;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetActorComponentsToTick() const noexcept
        {
            return _ActorComponentsToTick;
        }
        ABYTEK_FORCE_INLINE const auto& GetActorComponentsToPostTick() const noexcept
        {
            return _ActorComponentsToPostTick;
        }
        
    public:
        F_ActorComponentManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_ActorComponentManager() override;

    protected:
        void OnConfig() override;
        void OnPostConfig() override;
        
    protected:
        void OnInit() override;
        void OnStartup() override;
        void OnTick() override;
        void OnShutdown() override;
        void OnRelease() override;
        
    private:
        void _RegisterActorComponentToTick(const TW_Valid<A_ActorComponent>& Component);
        void _UnregisterActorComponentFromTick(const TW_Valid<A_ActorComponent>& Component);
        
    private:
        void _RegisterActorComponentToPostTick(const TW_Valid<A_ActorComponent>& Component);
        void _UnregisterActorComponentFromPostTick(const TW_Valid<A_ActorComponent>& Component);
        
    public:
        void Tick();
        void PostTick();
    };
}
