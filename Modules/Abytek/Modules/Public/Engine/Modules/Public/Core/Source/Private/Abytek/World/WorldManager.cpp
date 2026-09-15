#include "Abytek/World/WorldManager.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldSubsystem.hpp"
#include "Abytek/ApplicationModuleContainer.hpp"
#include "Abytek/Module.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_WorldManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_WorldManager"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_WorldManager)

    F_WorldManager::F_WorldManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
    }
    F_WorldManager::~F_WorldManager()
    {
    }

    void F_WorldManager::OnConfig()
    {
        _ConsoleVariable_MainWorld_Type = RegisterConsoleVariable<TF_ReflectionTypeHandle<F_World>>(
            ABYTEK_NAME("Abytek.MainWorld.Type"),
            ABYTEK_TEXT(""),
            TF_ReflectionTypeHandle<F_World>(F_ReflectionContext::GetGlobal())
        );
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        _ConsoleVariable_CookMode = RegisterConsoleVariable<B8>(
            ABYTEK_NAME("Abytek.CookMode"),
            ABYTEK_TEXT(""),
            false
        );
        _ConsoleVariable_CookProfile = RegisterConsoleVariable<F_Name>(
            ABYTEK_NAME("Abytek.CookProfile"),
            ABYTEK_TEXT(""),
            ABYTEK_NAME("Default")
        );
#endif
        _ConsoleVariable_PersistentLevel_Types = RegisterConsoleVariable<TF_Vector<TF_ReflectionTypeHandle<F_Level>>>(
            ABYTEK_NAME("Abytek.PersistentLevel.Types"),
            ABYTEK_TEXT(""),
            {}
        );
        _ConsoleVariable_PersistentLevel_Paths = RegisterConsoleVariable<TF_Vector<F_Name>>(
            ABYTEK_NAME("Abytek.PersistentLevel.Paths"),
            ABYTEK_TEXT(""),
            {}
        );
    }
    void F_WorldManager::OnPostConfig()
    {
    }

    void F_WorldManager::OnInit() 
    {
        {
            auto BaseSubsystemType = TF_ReflectionTypeHandle<A_WorldSubsystem>(F_ReflectionContext::GetGlobal());
            ABYTEK_ENGINE_NFC_ASSERT(BaseSubsystemType);
        
            F_ApplicationModuleContainer::GetInstance()->ForEachUnit(
                [&BaseSubsystemType, this](const TW_Valid<F_ProgramUnit>& Unit)
                {
                    auto Module = Unit.FastCast<F_Module>();
                    auto ReflectionSession = Module->GetReflectionSession();
                    ReflectionSession->ForEachTypeDerivedFrom(
                        BaseSubsystemType,
                        [this](const TW_Valid<F_ReflectionType>& Type)
                        {
                            _WorldSubsystemTypes.push_back(Type);
                            return true;
                        }
                    );
                    return true;
                }
            );
        }
    }
    void F_WorldManager::OnStartup() 
    {
    }
    void F_WorldManager::OnPostStartup() 
    {
        // Create main world
        {
            F_WorldConfig WorldConfig;
            WorldConfig.Flags = E_WorldFlag::DEFAULT | E_WorldFlag::MAIN;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
            if (_ConsoleVariable_CookMode->GetValue())
            {
                WorldConfig.Flags |= E_WorldFlag::COOK_MODE;
                WorldConfig.CookProfileName = _ConsoleVariable_CookProfile->GetValue();
            }
            else
#endif
            {
                // Main world's persistent levels
                {
                    auto PersistentLevelTypes = _ConsoleVariable_PersistentLevel_Types->GetValue();
                    auto PersistentLevelPaths = _ConsoleVariable_PersistentLevel_Paths->GetValue();
                    ABYTEK_ENGINE_NFC_ASSERT(
                        (PersistentLevelTypes.size() == PersistentLevelPaths.size())
                        || (PersistentLevelTypes.size() == 0)
                    );
                    if (PersistentLevelTypes.size() == 0)
                    {
                        for (const auto& LevelPath : PersistentLevelPaths)
                        {
                            PersistentLevelTypes.push_back(TF_ReflectionTypeHandle<F_Level>(F_ReflectionContext::GetGlobal()));
                        }
                    }
            
                    Sz NumPersistentLevels = PersistentLevelTypes.size();
                    for (Sz Idx = 0; Idx < NumPersistentLevels; ++Idx)
                    {
                        const auto& PersistentLevelType = PersistentLevelTypes[Idx];
                        const auto& PersistentLevelPath = PersistentLevelPaths[Idx];
                
                        F_WorldPersistentLevelInfo LevelInfo;
                        LevelInfo.Type = PersistentLevelType;
                        if (PersistentLevelPath)
                        {
                            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                                F_SerializableEnvironment::ParseObjectPath(
                                    PersistentLevelPath,
                                    LevelInfo.Name,
                                    LevelInfo.PackageName
                                )  
                            );
                        }
                
                        WorldConfig.PersistentLevels.push_back(LevelInfo);
                    }
                }
            }
            _MainWorld = F_World::Create(WorldConfig);
        }
    }
    void F_WorldManager::OnTick()
    {
    }
    void F_WorldManager::OnPreShutdown()
    {
    }
    void F_WorldManager::OnShutdown()
    {
    }
    void F_WorldManager::OnRelease() 
    {
    }
}
