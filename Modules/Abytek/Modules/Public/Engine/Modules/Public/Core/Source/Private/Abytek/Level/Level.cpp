#include "Abytek/Level/Level.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/Actor/Actor.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_Level)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Level"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_SerializedActors);
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(WorldBusinessType);
    }

    F_Level::F_Level(const F_SerializableObjectInitParams& InitParams) :
        A_WorldContext(InitParams)
    {
    }
    F_Level::~F_Level()
    {
    }

    void F_Level::OnLoad()
    {
        _Actors = _SerializedActors;
        _SerializedActors = {};
    }
    void F_Level::OnUnload()
    {
        _Actors = {};
    }

    void F_Level::OnLoadContent()
    {
    }
    void F_Level::OnUnloadContent()
    {
    }
    void F_Level::OnRegisterActorComponents()
    {
    }
    void F_Level::OnUnregisterActorComponents()
    {
    }
    void F_Level::OnBeginPlayActorComponents()
    {
    }
    void F_Level::OnEndPlayActorComponents()
    {
    }

    void F_Level::LoadContent()
    {
        _CriticalSection(
            [this]
            {
                ABYTEK_ENGINE_NFC_ASSERT(_State == E_LevelState::CONTENT_UNLOADED) << "Can only load levels in state E_LevelState::CONTENT_UNLOADED";
                GetWorld()->Events.OnLevelLoadContent.Broadcast(ABYTEK_WTHIS());
                OnLoadContent();
                GetWorld()->Events.OnLevelPostLoadContent.Broadcast(ABYTEK_WTHIS());
                _State = E_LevelState::CONTENT_LOADED;
            }
        );
    }
    void F_Level::UnloadContent()
    {
        _CriticalSection(
            [this]
            {
                ABYTEK_ENGINE_NFC_ASSERT(_State == E_LevelState::CONTENT_LOADED) << "Can only unload levels in state E_LevelState::CONTENT_LOADED";
                GetWorld()->Events.OnLevelPreUnloadContent.Broadcast(ABYTEK_WTHIS());
                OnUnloadContent();
                GetWorld()->Events.OnLevelUnloadContent.Broadcast(ABYTEK_WTHIS());
                _State = E_LevelState::CONTENT_UNLOADED;
            }
        );
    }
    void F_Level::RegisterActorComponents()
    {
        _CriticalSection(
            [this]
            {
                ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
                ABYTEK_CHECK_HIGH_LEVEL_UPDATE_RANGE();
                ABYTEK_ENGINE_NFC_ASSERT(_State == E_LevelState::CONTENT_LOADED) << "Can only activate levels in state E_LevelState::CONTENT_LOADED";
                GetWorld()->Events.OnLevelRegisterActorComponents.Broadcast(ABYTEK_WTHIS());
                OnRegisterActorComponents();
                for (const auto& Actor : _Actors)
                {
                    Actor->RegisterComponents();
                }
                GetWorld()->Events.OnLevelPostRegisterActorComponents.Broadcast(ABYTEK_WTHIS());
                _State = E_LevelState::ACTOR_COMPONENTS_REGISTERED;
            }
        );
    }
    void F_Level::UnregisterActorComponents()
    {
        _CriticalSection(
            [this]
            {
                ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
                ABYTEK_CHECK_HIGH_LEVEL_UPDATE_RANGE();
                ABYTEK_ENGINE_NFC_ASSERT(_State == E_LevelState::ACTOR_COMPONENTS_REGISTERED) << "Can only deactivate levels in state E_LevelState::ACTOR_COMPONENTS_REGISTERED";
                GetWorld()->Events.OnLevelPreUnregisterActorComponents.Broadcast(ABYTEK_WTHIS());
                for (const auto& Actor : _Actors)
                {
                    Actor->UnregisterComponents();
                }
                OnUnregisterActorComponents();
                GetWorld()->Events.OnLevelUnregisterActorComponents.Broadcast(ABYTEK_WTHIS());
                _State = E_LevelState::CONTENT_LOADED;
            }
        );
    }
    void F_Level::BeginPlayActorComponents()
    {
        _CriticalSection(
            [this]
            {
                ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
                ABYTEK_CHECK_HIGH_LEVEL_UPDATE_RANGE();
                ABYTEK_ENGINE_NFC_ASSERT(_State == E_LevelState::ACTOR_COMPONENTS_REGISTERED) << "Can only activate levels in state E_LevelState::ACTOR_COMPONENTS_REGISTERED";
                GetWorld()->Events.OnLevelBeginPlayActorComponents.Broadcast(ABYTEK_WTHIS());
                OnBeginPlayActorComponents();
                for (const auto& Actor : _Actors)
                {
                    Actor->BeginPlayComponents();
                }
                GetWorld()->Events.OnLevelPostBeginPlayActorComponents.Broadcast(ABYTEK_WTHIS());
                _State = E_LevelState::ACTOR_COMPONENTS_BEGAN_PLAY;
            }
        );
    }
    void F_Level::EndPlayActorComponents()
    {
        _CriticalSection(
            [this]
            {
                ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
                ABYTEK_CHECK_HIGH_LEVEL_UPDATE_RANGE();
                ABYTEK_ENGINE_NFC_ASSERT(_State == E_LevelState::ACTOR_COMPONENTS_BEGAN_PLAY) << "Can only deactivate levels in state E_LevelState::ACTOR_COMPONENTS_BEGAN_PLAY";
                GetWorld()->Events.OnLevelPreEndPlayActorComponents.Broadcast(ABYTEK_WTHIS());
                for (const auto& Actor : _Actors)
                {
                    Actor->EndPlayComponents();
                }
                OnEndPlayActorComponents();
                GetWorld()->Events.OnLevelEndPlayActorComponents.Broadcast(ABYTEK_WTHIS());
                _State = E_LevelState::ACTOR_COMPONENTS_REGISTERED;
            }
        );
    }

    B8 F_Level::HasActor(const TW_Valid<F_Actor>& Actor) const
    {
        for (const auto& A : _Actors)
        {
            if (A.Weak() == Actor)
            {
                return true;
            }
        }
        return false;
    }
    TW<F_Actor> F_Level::FindActor(const F_Name& ActorName) const
    {
        for (const auto& A : _Actors)
        {
            if (A->GetName() == ActorName)
            {
                return A.Weak();
            }
        }
        return {};
    }
    TW_Valid<F_Actor> F_Level::GetActor(const F_Name& ActorName) const
    {
        auto Actor = FindActor(ActorName);
        ABYTEK_ENGINE_NFC_ASSERT(Actor) << "Not found actor with name: " << ActorName;
        return Actor;
    }
    TW_Valid<F_Actor> F_Level::AddActor(const TS<F_Actor>& Actor)
    {
        ABYTEK_ENGINE_NFC_ASSERT(!HasActor(Actor.Weak())) << "Already added actor";
        ABYTEK_ENGINE_NFC_ASSERT(Actor->GetEnvironment() == GetEnvironment()) << "Environment mismatch";
        _Actors.push_back(Actor);
        Actor->_Level = ABYTEK_WTHIS();
        return Actor.Weak();
    }
    TS<F_Actor> F_Level::RemoveActor(const TW_Valid<F_Actor>& Actor)
    {
        ABYTEK_ENGINE_NFC_ASSERT(HasActor(Actor.Weak())) << "Not found actor";
        auto SActor = ShareObject(Actor);
        SActor->_Level = {};
        _Actors.erase(
            std::find(
                _Actors.begin(),
                _Actors.end(),
                SActor
            )  
        );
        return SActor;
    }

    void F_Level::OnPrepareForSaving(const TW_Valid<F_SerializableEnvironment>& Environment)
    {
        _SerializedActors = {};
        for (const auto& Actor : _Actors)
        {
            if (Actor->CanSerialize())
            {
                _SerializedActors.push_back(Actor);
            }
        }
    }
    void F_Level::OnCleanUpAfterSaving(const TW_Valid<F_SerializableEnvironment>& Environment)
    {
        _SerializedActors = {};
    }
}
