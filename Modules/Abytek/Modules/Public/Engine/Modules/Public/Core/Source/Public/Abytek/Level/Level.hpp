#pragma once

#include "Abytek/World/WorldContext.hpp"
#include "Abytek/Cookable.hpp"


namespace Abytek
{
    class F_Actor;
    class F_WorldBusiness;
    class F_Level;
    
    enum class E_LevelState : U8
    {
        CONTENT_UNLOADED,
        CONTENT_LOADED,
        ACTOR_COMPONENTS_REGISTERED,
        ACTOR_COMPONENTS_BEGAN_PLAY
    };
    
    struct F_LevelConfig
    {
    };
    struct F_LevelSetupParams : F_LevelConfig
    {
        TW<F_World> World;
    };
    class ABYTEK_ENGINE_CORE_API F_Level : public A_WorldContext, public I_Cookable
    {
    public:
        friend class F_LevelSubsystem;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(F_Level);
    
    private:
        E_LevelState _State = E_LevelState::CONTENT_UNLOADED;
        F_YieldReentrantCriticalSection _CriticalSection;
        
        TF_Vector<TS<F_Actor>> _Actors;
        TF_Vector<TS<F_Actor>> _SerializedActors;
        
    public:
        TF_ReflectionTypeHandle<F_WorldBusiness> WorldBusinessType;
        
    public:
        ABYTEK_FORCE_INLINE auto GetState() const noexcept
        {
            return _State;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetActors() const noexcept
        {
            return _Actors;
        }
        ABYTEK_FORCE_INLINE const auto& GetSerializedActors() const noexcept
        {
            return _SerializedActors;
        }
        
    public:
        F_Level(const F_SerializableObjectInitParams& InitParams);
        ~F_Level() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    protected:
        virtual void OnLoadContent();
        virtual void OnUnloadContent();
        virtual void OnRegisterActorComponents();
        virtual void OnUnregisterActorComponents();
        virtual void OnBeginPlayActorComponents();
        virtual void OnEndPlayActorComponents();
        
    public:
        // Parallelizable
        void LoadContent();
        // Parallelizable
        void UnloadContent();
        // Main thread only
        void RegisterActorComponents();
        // Main thread only
        void UnregisterActorComponents();
        // Main thread only
        void BeginPlayActorComponents();
        // Main thread only
        void EndPlayActorComponents();
        
    public:
        B8 HasActor(const TW_Valid<F_Actor>& Actor) const;
        TW<F_Actor> FindActor(const F_Name& ActorName) const;
        TW_Valid<F_Actor> GetActor(const F_Name& ActorName) const;
        TW_Valid<F_Actor> AddActor(const TS<F_Actor>& Actor);
        TS<F_Actor> RemoveActor(const TW_Valid<F_Actor>& Actor);
        
    protected:
        void OnPrepareForSaving(const TW_Valid<F_SerializableEnvironment>& Environment) override;
        void OnCleanUpAfterSaving(const TW_Valid<F_SerializableEnvironment>& Environment) override;
    };
}