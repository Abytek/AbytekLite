#include "Abytek/World/World.hpp"
#include "Abytek/ApplicationModuleContainer.hpp"
#include "Abytek/ApplicationModule.hpp"
#include "Abytek/ApplicationSubsystem.hpp"
#include "Abytek/World/WorldBusiness.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/World/WorldManager.hpp"
#include "Abytek/World/WorldSubsystem.hpp"
#include "Abytek/World/WorldSubsystemContainer.hpp"
#include "Abytek/ApplicationUpdateFunction.hpp"
#include "Abytek/CoreUpdateGraph/PrimaryUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/Level/Level.hpp"
#include "Abytek/CoreUpdateGraph/PostTickUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/ShutdownUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/StartupUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/TickUpdateRange.hpp"
#include "Abytek/Development/Cook/CookGraph.hpp"
#include "Abytek/Development/Cook/CookProfile.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_World)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_World"));
    }

    F_Name F_World::GetCookTaskTag()
    {
        return ABYTEK_NAME("Abytek::F_World::Cook");
    }

    F_Name F_World::GetInitUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_World::Init");
    }
    F_Name F_World::GetStartupUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_World::Startup");
    }
    F_Name F_World::GetShutdownUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_World::Shutdown");
    }
    F_Name F_World::GetReleaseUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_World::Release");
    }

    void F_World::GlobalInit()
    {
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    for (const auto& WorldStart : F_WorldManager::GetInstance()->_WorldsToInit.PopAll())
                    {
                        auto World = WorldStart.World;
                        auto SerializableEnvironment = World->GetEnvironment();
                        const auto& Config = WorldStart.Config;
                        
                        auto& Worlds = F_WorldManager::GetInstance()->_Worlds;
                        Worlds.push_back(World.Weak());
                        
                        World->_Flags = Config.Flags;
                        if (FlagHas(World->_Flags, E_WorldFlag::MAIN))
                        {
                            ABYTEK_ENGINE_CORE_ASSERT(!_Main) << "Multiple main worlds are not allowed";
                            _Main = World.Weak();
                        }
                        
                        World->_SubsystemContainer = TU<F_WorldSubsystemContainer>()(World.Weak());
                        
                        // Init subsystems
                        {
                            for (const auto& WorldSubsystemType : F_WorldManager::GetInstance()->GetWorldSubsystemTypes())
                            {
                                World->_SubsystemContainer->EnsureUnit(WorldSubsystemType);
                            }
                            World->_SubsystemContainer->Update();
                        }
                    }
                },
                GetInitUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                A_ApplicationSubsystem::GetStartupUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_StartupUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    for (const auto& WorldStart : F_WorldManager::GetInstance()->_WorldsToStartup.PopAll())
                    {
                        auto World = WorldStart.World;
                        auto SerializableEnvironment = World->GetEnvironment();
                        const auto& Config = WorldStart.Config;
                        
                        // Create CDOs
                        {
                            auto BaseCDOType = TF_ReflectionTypeHandle<A_SerializableObject>(F_ReflectionContext::GetGlobal());
                            ABYTEK_ENGINE_NFC_ASSERT(BaseCDOType);
                            F_ApplicationModuleContainer::GetInstance()->ForEachUnit(
                                [&](const TW_Valid<F_ProgramUnit>& Unit)
                                {
                                    auto Module = Unit.FastCast<F_Module>();
                                    auto ReflectionSession = Module->GetReflectionSession();
                                    ReflectionSession->ForEachTypeDerivedFrom(
                                        BaseCDOType,
                                        [&](const TW_Valid<F_ReflectionType>& Type)
                                        {
                                            if (Type->IsAbstract())
                                            {
                                                return true;
                                            }
                                            World->_CDOTypes.push_back(Type);
                                            return true;
                                        }
                                    );
                                    return true;
                                }
                            );
                            for (const auto& Type : World->_CDOTypes)
                            {
                                SerializableEnvironment->AddCDOType(Type);
                            }
                        }
                        
                        // Load objects
                        SerializableEnvironment->LoadEnqueuedObjects();
                        
                        // Startup subsystems
                        {
                            World->_SubsystemContainer->ForEachUnit(
                                [](const TW_Valid<F_ProgramUnit>& Unit)
                                {
                                    Unit.FastCast<A_WorldSubsystem>()->OnStartup();
                                    return true;
                                }
                            );
                            World->_SubsystemContainer->ForEachUnit(
                                [](const TW_Valid<F_ProgramUnit>& Unit)
                                {
                                    Unit.FastCast<A_WorldSubsystem>()->OnPostStartup();
                                    return true;
                                }
                            );
                        }
                        
                        // Persistent business
                        if (!World->HasFlags(E_WorldFlag::COOK_MODE))
                        {
                            auto PersistentBusinessType = Config.PersistentBusinessType;
                            if (!PersistentBusinessType)
                            {
                                PersistentBusinessType = TF_ReflectionTypeHandle<F_WorldBusiness>(F_ReflectionContext::GetGlobal());
                            }
                            World->_PersistentBusiness = H_WorldContext::CreateObject<F_WorldBusiness>(
                                World.Weak(),
                                {},
                                {},
                                PersistentBusinessType
                            );
                            World->_PersistentBusiness->BeginPlay();
                        }
                        
                        // Cook
                        if (FlagHas(World->_Flags, E_WorldFlag::COOK_MODE))
                        {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                            ABYTEK_ENGINE_CORE_ASSERT(FlagHas(World->_Flags, E_WorldFlag::MAIN)) << "Cook mode is only allowed on main world";
                            ABYTEK_ENGINE_CORE_ASSERT(Config.CookProfileName) << "Invalid cook profile";
                            World->_CookProfile = TU<F_CookProfile>()(
                                Config.CookProfileName,
                                A_ApplicationCore::GetInstance()->GetName()
                            );
                            F_CookProfile::SetMain(World->_CookProfile.Weak());
                            F_HighLevelUpdateRange::EnqueueCommand(
                                [World = World]
                                {
                                    World->_Cook();
                                }
                            );
#else
                            ABYTEK_LOG_FATAL() << "Cook mode is not allowed on non-development build";
#endif
                        }
                        
                        // Travel level
                        {
                            TF_Vector<TS<F_Level>> Levels;
                            for (const auto& PersistentLevel : Config.PersistentLevels)
                            {
                                auto Level = H_WorldContext::CreateObject<F_Level>(
                                    World.Weak(),
                                    PersistentLevel.Name,
                                    PersistentLevel.PackageName,
                                    PersistentLevel.Type.Cast<F_Level>()
                                );
                                Levels.push_back(Level);
                            }
                            World->_LevelsToTravel = Levels;
                            World->_TravelMode = E_TravelMode::START;
                            World->_ImmediateTravel();
                        }
                        
                        // Callbacks
                        TF_Function<void(const TS<F_World>& World)> Command;
                        while (World->_StartCallbacks.TryPop(Command))
                        {
                            Command(World);
                        }
                    }
                },
                GetStartupUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                GetInitUpdateFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                F_StartupUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    for (const auto& World : F_WorldManager::GetInstance()->_WorldsToShutdown.PopAll())
                    {
                        auto SerializableEnvironment = World->GetEnvironment();
                        
                        // Cook
                        if (FlagHas(World->_Flags, E_WorldFlag::COOK_MODE))
                        {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                            ABYTEK_ENGINE_CORE_ASSERT(FlagHas(World->_Flags, E_WorldFlag::MAIN)) << "Cook mode is only allowed on main world";
                            World->_CookProfile = {};
                            F_CookProfile::SetMain({});
#else
                            ABYTEK_LOG_FATAL() << "Cook mode is not allowed on non-development build";
#endif
                        }
                        
                        World->_LevelsToTravel = {};
                        World->_TravelMode = E_TravelMode::STOP;
                        World->_ImmediateTravel();
                        
                        // Persistent business
                        if (World->_PersistentBusiness)
                        {
                            World->_PersistentBusiness->EndPlay();
                            World->_PersistentBusiness = {};
                        }
                        
                        // Shutdown subsystems
                        {
                            World->_SubsystemContainer->ForEachUnit(
                                [](const TW_Valid<F_ProgramUnit>& Unit)
                                {
                                    Unit.FastCast<A_WorldSubsystem>()->OnPreShutdown();
                                    return true;
                                }
                            );
                            World->_SubsystemContainer->ForEachUnit(
                                [](const TW_Valid<F_ProgramUnit>& Unit)
                                {
                                    Unit.FastCast<A_WorldSubsystem>()->OnShutdown();
                                    return true;
                                }
                            );
                        }
                        
                        // Destroy CDOs
                        {
                            for (const auto& Type : World->_CDOTypes)
                            {
                                SerializableEnvironment->RemoveCDOType(Type);
                            }
                            World->_CDOTypes = {};
                        }
                    }
                },
                GetShutdownUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                F_ShutdownUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                A_ApplicationSubsystem::GetShutdownUpdateFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    for (const auto& World : F_WorldManager::GetInstance()->_WorldsToRelease.PopAll())
                    {
                        auto SerializableEnvironment = World->GetEnvironment();
                        
                        // Release subsystems
                        {
                            World->_SubsystemContainer->EnqueueRemoveAllUnits();
                            World->_SubsystemContainer->Update();
                        }
                        
                        World->_SubsystemContainer = {};
                        
                        B8 IsMain = FlagHas(World->_Flags, E_WorldFlag::MAIN);
                        if (IsMain)
                        {
                            _Main = {};
                        }
                        World->_Flags = E_WorldFlag::NONE;
                        
                        auto& Worlds = F_WorldManager::GetInstance()->_Worlds;
                        Worlds.erase(
                            std::find(
                                Worlds.begin(),
                                Worlds.end(),
                                World.Weak()
                            )    
                        );
                        ABYTEK_ENGINE_CORE_ASSERT(
                            (Worlds.size() == 0)
                            || !IsMain
                        ) << "Main world must be the last world to stop";
                        
                        if (IsMain)
                        {
                            A_ApplicationCore::GetInstance()->SignalShutdown();
                        }
                        
                        // Callbacks
                        TF_Function<void()> Command;
                        while (World->_StopCallbacks.TryPop(Command))
                        {
                            Command();
                        }
                    }
                },
                GetReleaseUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                GetShutdownUpdateFunctionName()
            );
            UpdateFunction->AddReverseDependency(
                A_ApplicationSubsystem::GetShutdownUpdateFunctionName()  
            );
        }
    }
    void F_World::GlobalRelease()
    {
        H_UpdateUtilities::UnregisterFunction(
            GetReleaseUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetShutdownUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetStartupUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetInitUpdateFunctionName()
        );
    }

    TW<F_World> F_World::_Main;

    TS<F_World> F_World::Create(const F_WorldConfig& Config, TF_Function<void(const TS<F_World>& World)>&& Callback)
    {
        auto Result = F_EngineRuntime::GetInstance()->GetSerializableEnvironment()->CreateObject<F_World>(
            Config.Name
        );
        if (Callback)
        {
            Result->_StartCallbacks.Push(ABYTEK_MOVE(Callback));
        }
        
        F_HighLevelUpdateRange::EnqueueCommand(
            [Result, Config]
            {
                Result->_PrepareStart(Config);
            }
        );
        return Result;
    }

    F_World::F_World(const F_SerializableObjectInitParams& InitParams) :
        A_WorldContext(InitParams)
    {
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            _OwnedSerializableEnvironment = ShareObject(GetEnvironment());
            _OwnedSerializableEnvironment->AssignOwner(ABYTEK_WTHIS());
        }
    }
    F_World::~F_World()
    {
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            _OwnedSerializableEnvironment = {};
        }
    }

    void F_World::OnLoad()
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
    }
    void F_World::OnUnload()
    {
    }

    void F_World::_PrepareStart(const F_WorldConfig& Config)
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
        
        auto WorldManager = F_WorldManager::GetInstance();
        F_WorldManager::F_WorldStart WorldStart;
        WorldStart.World = ABYTEK_STHIS();
        WorldStart.Config = Config;
        WorldManager->_WorldsToInit.Push(WorldStart);
        WorldManager->_WorldsToStartup.Push(WorldStart);
    }
    void F_World::_PrepareStop()
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
        
        auto WorldManager = F_WorldManager::GetInstance();
        WorldManager->_WorldsToShutdown.Push(ABYTEK_STHIS());
        WorldManager->_WorldsToRelease.Push(ABYTEK_STHIS());
    }

    void F_World::Stop(TF_Function<void()>&& Callback)
    {
        if (Callback)
        {
            _StopCallbacks.Push(ABYTEK_MOVE(Callback));
        }
        if (_RequestedToStop)
        {
            return;
        }
        _RequestedToStop = true;
        
        F_HighLevelUpdateRange::EnqueueCommand(
            [this]
            {
                _PrepareStop();
            }
        );
    }

    B8 F_World::HasLevel(const TW_Valid<F_Level>& Level)
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
        auto SLevel = ShareObject(Level);
        if (!SLevel)
        {
            return {};
        }
        return _Levels.find(SLevel) != _Levels.end();
    }
    void F_World::AddLevel(const TS_Valid<F_Level>& Level)
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
        ABYTEK_ENGINE_NFC_ASSERT(!HasLevel(Level.Weak())) << "Already added level: " << Level->GetName();
        ABYTEK_ENGINE_NFC_ASSERT(Level->GetState() == E_LevelState::CONTENT_UNLOADED) << "Requires level in state E_LevelState::CONTENT_UNLOADED, current level sate: " << static_cast<U32>(Level->GetState());
        ABYTEK_ENGINE_NFC_ASSERT(Level->GetEnvironment() == GetEnvironment()) << "Environment mismatch";
        _Levels.insert(Level);
        Events.OnAddLevel.Broadcast(Level.Weak());
    }
    void F_World::RemoveLevel(const TW_Valid<F_Level>& Level)
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
        auto SLevel = ShareObject(Level);
        ABYTEK_ENGINE_NFC_ASSERT(SLevel) << "Invalid level";
        ABYTEK_ENGINE_NFC_ASSERT(HasLevel(Level.Weak())) << "Not found level: " << Level->GetName();
        Events.OnRemoveLevel.Broadcast(Level);
        _Levels.erase(_Levels.find(SLevel));
    }
    void F_World::RemoveAllLevels()
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
        while (_Levels.size() > 0)
        {
            auto Level = _Levels.begin()->Weak();
            RemoveLevel(Level);
        }
    }

    B8 F_World::HasPersistentLevel(const TW_Valid<F_Level>& Level)
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
        auto SLevel = ShareObject(Level);
        if (!SLevel)
        {
            return {};
        }
        return _PersistentLevels.find(SLevel) != _PersistentLevels.end();
    }
    void F_World::AddPersistentLevel(const TS_Valid<F_Level>& Level)
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
        ABYTEK_ENGINE_NFC_ASSERT(_CanModifyPersistentLevels);
        ABYTEK_ENGINE_NFC_ASSERT(_PersistentLevels.find(Level) == _PersistentLevels.end());
        _PersistentLevels.insert(Level);
        if (!HasLevel(Level.Weak()))
        {
            AddLevel(Level);
        }
    }
    void F_World::RemovePersistentLevel(const TW_Valid<F_Level>& Level)
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
        ABYTEK_ENGINE_NFC_ASSERT(_CanModifyPersistentLevels);
        auto SLevel = ShareObject(Level);
        ABYTEK_ENGINE_NFC_ASSERT(_PersistentLevels.find(SLevel) != _PersistentLevels.end());
        _PersistentLevels.erase(_PersistentLevels.find(SLevel));
        if (HasLevel(Level))
        {
            RemoveLevel(Level);
        }
    }

    void F_World::_ImmediateTravel()
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
        ABYTEK_CHECK_HIGH_LEVEL_UPDATE_RANGE();
        
        auto LevelsToTravel = ABYTEK_MOVE(_LevelsToTravel);
        auto TravelMode = _TravelMode;
        
        // End play actor components
        for (const auto& Level : _PersistentLevels)
        {
            if (Level->GetState() < E_LevelState::ACTOR_COMPONENTS_BEGAN_PLAY)
            {
                continue;
            }
            Level->EndPlayActorComponents();
        }
        
        // End play session business
        if (_SessionBusiness)
        {
            _SessionBusiness->EndPlay();
            _SessionBusiness = {};
        }
        
        // Unregister actor components
        for (const auto& Level : _PersistentLevels)
        {
            if (Level->GetState() < E_LevelState::ACTOR_COMPONENTS_BEGAN_PLAY)
            {
                continue;
            }
            Level->UnregisterActorComponents();
        }
        
        // Unload content
        for (const auto& Level : _PersistentLevels)
        {
            if (Level->GetState() < E_LevelState::CONTENT_LOADED)
            {
                continue;
            }
            Level->UnloadContent();
        }
        
        // Remove persistent levels
        {
            _CanModifyPersistentLevels = true;
            while (_PersistentLevels.size() > 0)
            {
                RemovePersistentLevel(_PersistentLevels.begin()->Weak());
            }
            _CanModifyPersistentLevels = false;
        }
        
        // Stop if needed
        if (TravelMode == E_TravelMode::STOP)
        {
            return;
        }
                
        // Add persistent levels
        {
            _CanModifyPersistentLevels = true;
            for (const auto& Level : LevelsToTravel)
            {
                AddPersistentLevel(Level);
            }
            _CanModifyPersistentLevels = false;
        }
        
        // Load content
        {
            for (const auto& Level : _PersistentLevels)
            {
                if (Level->GetState() >= E_LevelState::CONTENT_LOADED)
                {
                    continue;
                }
                Level->LoadContent();
            }
        }
        
        // Register actor components
        {
            for (const auto& Level : _PersistentLevels)
            {
                if (Level->GetState() >= E_LevelState::ACTOR_COMPONENTS_REGISTERED)
                {
                    continue;
                }
                Level->RegisterActorComponents();
            }
        }
        
        // Begin play session business
        if (!HasFlags(E_WorldFlag::COOK_MODE))
        {
            TF_ReflectionTypeHandle<F_WorldBusiness> BusinessType(F_ReflectionContext::GetGlobal());
            for (const auto& Level : LevelsToTravel)
            {
                if (auto LevelBusinessType = Level->WorldBusinessType)
                {
                    BusinessType = LevelBusinessType;
                }
            }
            _SessionBusiness = H_WorldContext::CreateObject<F_WorldBusiness>(
                ABYTEK_WTHIS(),
                {},
                {},
                BusinessType
            );
            _SessionBusiness->BeginPlay();
        }
        
        // Begin play actor components
        {
            for (const auto& Level : _PersistentLevels)
            {
                if (Level->GetState() >= E_LevelState::ACTOR_COMPONENTS_BEGAN_PLAY)
                {
                    continue;
                }
                Level->BeginPlayActorComponents();
            }
        }
    }

    void F_World::Travel(const TF_Vector<TS<F_Level>>& Levels)
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::MAIN);
        ABYTEK_ENGINE_CORE_ASSERT(!_RequestedToStop) << "Cannot travel after having requested to stop";
        _LevelsToTravel = Levels;
        _TravelMode = E_TravelMode::NORMAL;
        if (_RequestedToTravel)
        {
            return;
        }
        _RequestedToTravel = true;
        F_HighLevelUpdateRange::EnqueueCommand(
            [this]
            {
                _ImmediateTravel();
            }
        );
    }
    void F_World::Travel(const TS<F_Level>& Level)
    {
        TF_Vector<TS<F_Level>> Levels;
        Levels.push_back(Level);
        Travel(Levels);
    }
    void F_World::Travel(const F_Name& LevelPath, const TF_ReflectionTypeHandle<F_Level>& LevelType)
    {
        F_Name LevelName;
        F_Name LevelPackageName;
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            F_SerializableEnvironment::ParseObjectPath(LevelPath, LevelName, LevelPackageName)    
        );
        
        auto Level = H_WorldContext::CreateObject<F_Level>(
            ABYTEK_WTHIS(),
            LevelName,
            LevelPackageName,
            LevelType
        );
        Travel(Level);
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void F_World::_Cook()
    {
        _BuildConfigDirectory();
        
        H_TaskUtilities::Schedule(
            [this]
            {
                H_TaskUtilities::AddTag(GetCookTaskTag());
                
                F_CookGraph::GetInstance()->Update();
                
                ABYTEK_LOG_INFO() << "Successfully cooked application \"" << A_ApplicationCore::GetInstance()->GetName() << "\"";
                
                F_HighLevelUpdateRange::EnqueueCommand(
                    [this]
                    {
                        Stop();
                    }
                );
                
                H_TaskUtilities::RemoveTag(GetCookTaskTag());
            },
            E_TaskPriority::DEFAULT,
            ABYTEK_NAME("Cook")
        );
    }
    void F_World::_BuildConfigDirectory()
    {
        auto CookProfile = F_CookProfile::GetMain();
        const auto& IntermediateConfigsDirectoryPath = CookProfile->GetIntermediateConfigsDirectoryPath();
        
        for (const auto& [ConfigFilePath, ConfigFileContent] : F_EngineRuntime::GetInstance()->GetConsole()->GetConfigFiles())
        {
            F_Text BaseDirectory = IntermediateConfigsDirectoryPath + ABYTEK_TEXT("/") + ToText(boost::hash<F_Text>()(ConfigFilePath));
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_FSUtilities::EnsureDirectory(BaseDirectory)  
            );
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_FSUtilities::WriteFileText(
                    BaseDirectory + ABYTEK_TEXT("/") + F_EngineRuntime::GetConfigFileName(),
                    ConfigFileContent
                )  
            );
        }
        for (const auto& [ConfigFilePath, ConfigFileContent] : A_ApplicationCore::GetInstance()->GetConsole()->GetConfigFiles())
        {
            F_Text BaseDirectory = IntermediateConfigsDirectoryPath + ABYTEK_TEXT("/") + ToText(boost::hash<F_Text>()(ConfigFilePath));
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_FSUtilities::EnsureDirectory(BaseDirectory)  
            );
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_FSUtilities::WriteFileText(
                    BaseDirectory + ABYTEK_TEXT("/") + A_ApplicationCore::GetConfigFileName(),
                    ConfigFileContent
                )  
            );
        }
    }
#endif
}
