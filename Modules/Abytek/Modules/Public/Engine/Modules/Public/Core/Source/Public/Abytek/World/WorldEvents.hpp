#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


namespace Abytek
{
    class F_Level;
    
    namespace WorldEvents
    {
        using F_OnAddLevel = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        using F_OnRemoveLevel = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        
        using F_OnLevelLoadContent = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        using F_OnLevelPostLoadContent = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        using F_OnLevelUnloadContent = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        using F_OnLevelPreUnloadContent = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        
        using F_OnLevelRegisterActorComponents = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        using F_OnLevelPostRegisterActorComponents = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        using F_OnLevelUnregisterActorComponents = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        using F_OnLevelPreUnregisterActorComponents = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        
        using F_OnLevelBeginPlayActorComponents = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        using F_OnLevelPostBeginPlayActorComponents = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        using F_OnLevelEndPlayActorComponents = TF_Event<void(const TW_Valid<F_Level>& Level)>;
        using F_OnLevelPreEndPlayActorComponents = TF_Event<void(const TW_Valid<F_Level>& Level)>;
    }
    struct F_WorldEvents 
    {
        WorldEvents::F_OnAddLevel OnAddLevel;
        WorldEvents::F_OnRemoveLevel OnRemoveLevel;
        
        WorldEvents::F_OnLevelLoadContent OnLevelLoadContent;
        WorldEvents::F_OnLevelPostLoadContent OnLevelPostLoadContent;
        WorldEvents::F_OnLevelUnloadContent OnLevelUnloadContent;
        WorldEvents::F_OnLevelPreUnloadContent OnLevelPreUnloadContent;
        
        WorldEvents::F_OnLevelRegisterActorComponents OnLevelRegisterActorComponents;
        WorldEvents::F_OnLevelPostRegisterActorComponents OnLevelPostRegisterActorComponents;
        WorldEvents::F_OnLevelUnregisterActorComponents OnLevelUnregisterActorComponents;
        WorldEvents::F_OnLevelPreUnregisterActorComponents OnLevelPreUnregisterActorComponents;
        
        WorldEvents::F_OnLevelBeginPlayActorComponents OnLevelBeginPlayActorComponents;
        WorldEvents::F_OnLevelPostBeginPlayActorComponents OnLevelPostBeginPlayActorComponents;
        WorldEvents::F_OnLevelEndPlayActorComponents OnLevelEndPlayActorComponents;
        WorldEvents::F_OnLevelPreEndPlayActorComponents OnLevelPreEndPlayActorComponents;
    };
}