#include "Abytek/Actor/ActorComponent.hpp"
#include "Abytek/ApplicationUpdateFunction.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/Actor/Actor.hpp"
#include "Abytek/Actor/ActorComponentManager.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PostTickUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/TickUpdateRange.hpp"
#include "Abytek/World/WorldSubsystem.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_ActorComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_ActorComponent"));
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Actor);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Tags);
    }

    F_Name A_ActorComponent::GetTickUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ActorComponent::Tick");
    }
    F_Name A_ActorComponent::GetPostTickUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ActorComponent::PostTick");
    }
    void A_ActorComponent::GlobalInit()
    {
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ActorComponentManager::GetInstance()->Tick();
                },
                GetTickUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                A_WorldSubsystem::GetTickUpdateFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_TickUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ActorComponentManager::GetInstance()->PostTick();
                },
                GetPostTickUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddReverseDependency(
                A_WorldSubsystem::GetPostTickUpdateFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_PostTickUpdateRange::GetEndFunctionName()
            );
        }
    }
    void A_ActorComponent::GlobalRelease()
    {
        H_UpdateUtilities::UnregisterFunction(
            GetTickUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetPostTickUpdateFunctionName()
        );
    }
    
    A_ActorComponent::A_ActorComponent(const F_SerializableObjectInitParams& InitParams) :
        A_WorldContext(InitParams)
    {
    }
    A_ActorComponent::~A_ActorComponent()
    {
    }

    void A_ActorComponent::OnAddToActor()
    {
    }
    void A_ActorComponent::OnRemoveFromActor()
    {
    }
    void A_ActorComponent::OnRegisterComponent()
    {
    }
    void A_ActorComponent::OnUnregisterComponent()
    {
    }
    void A_ActorComponent::OnBeginPlayComponent()
    {
    }
    void A_ActorComponent::OnEndPlayComponent()
    {
    }
    void A_ActorComponent::OnTick()
    {
    }
    void A_ActorComponent::OnPostTick()
    {
    }

    void A_ActorComponent::RegisterComponent()
    {
        ABYTEK_ENGINE_NFC_ASSERT(!_IsRegistered);
        _IsRegistered = true;
        if (_IsEnabledTick)
        {
            _ActualEnableTick();
        }
        if (_IsEnabledPostTick)
        {
            _ActualEnablePostTick();
        }
        OnRegisterComponent();
        for (const auto& Component : _ChildInstanceComponents)
        {
            Component->RegisterComponent();
        }
    }
    void A_ActorComponent::UnregisterComponent()
    {
        ABYTEK_ENGINE_NFC_ASSERT(_IsRegistered);
        for (const auto& Component : _ChildInstanceComponents)
        {
            Component->UnregisterComponent();
        }
        OnUnregisterComponent();
        if (_IsEnabledTick)
        {
            _ActualDisablePostTick();
        }
        if (_IsEnabledTick)
        {
            _ActualDisableTick();
        }
        _IsRegistered = false;
    }
    void A_ActorComponent::BeginPlayComponent()
    {
        ABYTEK_ENGINE_NFC_ASSERT(!_BeganPlay);
        _BeganPlay = true;
        OnBeginPlayComponent();
        for (const auto& Component : _ChildInstanceComponents)
        {
            Component->BeginPlayComponent();
        }
    }
    void A_ActorComponent::EndPlayComponent()
    {
        ABYTEK_ENGINE_NFC_ASSERT(_BeganPlay);
        for (const auto& Component : _ChildInstanceComponents)
        {
            Component->EndPlayComponent();
        }
        OnEndPlayComponent();
        _BeganPlay = false;
    }

    void A_ActorComponent::MakeRuntimeComponentReady()
    {
        auto Actor = GetActor();
        ABYTEK_ENGINE_NFC_ASSERT(Actor);
        auto Level = Actor->GetLevel();
        ABYTEK_ENGINE_NFC_ASSERT(Level);
        if (Level->GetState() >= E_LevelState::ACTOR_COMPONENTS_REGISTERED)
        {
            RegisterComponent();
        }
        if (Level->GetState() >= E_LevelState::ACTOR_COMPONENTS_BEGAN_PLAY)
        {
            BeginPlayComponent();
        }
    }

    void A_ActorComponent::EnableTick()
    {
        if (_IsEnabledTick)
        {
            return;
        }
        _IsEnabledTick = true;
        if (IsRegistered() && !HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            _ActualEnableTick();
        }
    }
    void A_ActorComponent::DisableTick()
    {
        if (!_IsEnabledTick)
        {
            return;
        }
        _IsEnabledTick = false;
        if (IsRegistered() && !HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            _ActualDisableTick();
        }
    }
    void A_ActorComponent::EnablePostTick()
    {
        if (_IsEnabledPostTick)
        {
            return;
        }
        _IsEnabledPostTick = true;
        if (IsRegistered() && !HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            _ActualEnablePostTick();
        }
    }
    void A_ActorComponent::DisablePostTick()
    {
        if (!_IsEnabledPostTick)
        {
            return;
        }
        _IsEnabledPostTick = false;
        if (IsRegistered() && !HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            _ActualDisablePostTick();
        }
    }

    void A_ActorComponent::_ActualEnableTick()
    {
        F_ActorComponentManager::GetInstance()->_RegisterActorComponentToTick(ABYTEK_WTHIS());
    }
    void A_ActorComponent::_ActualDisableTick()
    {
        F_ActorComponentManager::GetInstance()->_UnregisterActorComponentFromTick(ABYTEK_WTHIS());
    }
    void A_ActorComponent::_ActualEnablePostTick()
    {
        F_ActorComponentManager::GetInstance()->_RegisterActorComponentToPostTick(ABYTEK_WTHIS());
    }
    void A_ActorComponent::_ActualDisablePostTick()
    {
        F_ActorComponentManager::GetInstance()->_UnregisterActorComponentFromPostTick(ABYTEK_WTHIS());
    }

    void A_ActorComponent::AddChildInstanceComponent(const TS<A_ActorComponent>& Component)
    {
        ABYTEK_ENGINE_NFC_ASSERT(!_IsRegistered);
        if (
            std::find(
                _ChildInstanceComponents.begin(), 
                _ChildInstanceComponents.end(), 
                Component
            ) 
            != _ChildInstanceComponents.end()
        )
        {
            return;
        }
        _ChildInstanceComponents.push_back(Component);
    }

    void A_ActorComponent::_AddToActor(const TW_Valid<F_Actor>& Actor)
    {
        OnAddToActor();
        for (const auto& Component : _ChildInstanceComponents)
        {
            Actor->AddInstanceComponent(Component);
        }
    }
    void A_ActorComponent::_RemoveFromActor()
    {
        auto Actor = GetActor();
        for (const auto& Component : _ChildInstanceComponents)
        {
            Actor->RemoveInstanceComponent(Component.Weak());
        }
        OnRemoveFromActor();
    }
}
