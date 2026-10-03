#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ApplicationSubsystem.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    class A_WorldSubsystem;
    class F_Level;
    struct F_WorldStart
    {
        TS<F_World> World;
        F_WorldConfig Config;
    };
    
    class ABYTEK_ENGINE_CORE_API F_WorldManager final : public A_ApplicationSubsystem
    {
    public:
        friend class F_World;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_WorldManager);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_WorldManager);

    private:
        TW<TF_ConsoleVariable<TF_ReflectionTypeHandle<F_World>>> _ConsoleVariable_MainWorld_Type;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TW<TF_ConsoleVariable<B8>> _ConsoleVariable_CookMode;
        TW<TF_ConsoleVariable<F_Name>> _ConsoleVariable_CookProfile;
#endif
        TW<TF_ConsoleVariable<TF_Vector<TF_ReflectionTypeHandle<F_Level>>>> _ConsoleVariable_PersistentLevel_Types;
        TW<TF_ConsoleVariable<TF_Vector<F_Name>>> _ConsoleVariable_PersistentLevel_Paths;
        
        TF_Vector<TW<F_World>> _Worlds;
        TS<F_World> _MainWorld;
        
        struct F_WorldStart
        {
            TS<F_World> World;
            F_WorldConfig Config;
        };
        TF_Queue<F_WorldStart> _WorldsToInit;
        TF_Queue<F_WorldStart> _WorldsToStartup;
        TF_Queue<TS<F_World>> _WorldsToShutdown;
        TF_Queue<TS<F_World>> _WorldsToRelease;
        
        TF_Vector<TF_ReflectionTypeHandle<A_WorldSubsystem>> _WorldSubsystemTypes;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetWorlds() const noexcept
        {
            return _Worlds;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetWorldSubsystemTypes() const noexcept
        {
            return _WorldSubsystemTypes;
        }
        
    public:
        F_WorldManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_WorldManager() override;

    protected:
        void OnConfig() override;
        void OnPostConfig() override;
        
    protected:
        void OnInit() override;
        void OnStartup() override;
        void OnPostStartup() override;
        void OnTick() override;
        void OnPreShutdown() override;
        void OnShutdown() override;
        void OnRelease() override;
    };
}
