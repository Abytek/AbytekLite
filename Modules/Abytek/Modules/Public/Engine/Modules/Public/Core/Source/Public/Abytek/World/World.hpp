#pragma once

#include "Abytek/World/WorldContext.hpp"
#include "Abytek/Level/Level.hpp"
#include "Abytek/World/WorldEvents.hpp"


namespace Abytek
{
    class F_Level;
    class F_WorldSubsystemContainer;
    class F_WorldBusiness;
    
    struct F_WorldPersistentLevelInfo : F_LevelConfig, F_SerializableObjectCreationParams
    {
    };
    
    enum class E_WorldFlag : U32
    {
        NONE = 0x0,
        
        MAIN = 0x1,
        
        CREATE_PHYSICS_SCENE = 0x2,
        CREATE_RENDER_SCENE = 0x4,
        CREATE_SCENES = CREATE_PHYSICS_SCENE | CREATE_RENDER_SCENE,
        
        SIMULATE_PHYSICS = 0x8 | CREATE_PHYSICS_SCENE,
        ENABLE_RENDERING = 0x10 | CREATE_RENDER_SCENE,
        
        COOK_MODE = 0x20,
        
        DEFAULT = CREATE_SCENES | SIMULATE_PHYSICS | ENABLE_RENDERING
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_WorldFlag);
    
    struct F_WorldConfig
    {
        F_Name Name;
        E_WorldFlag Flags = E_WorldFlag::DEFAULT;
        F_Name CookProfileName;
        TF_ReflectionTypeHandle<F_WorldBusiness> PersistentBusinessType;
        TF_Vector<F_WorldPersistentLevelInfo> PersistentLevels;
    };
    
    class ABYTEK_ENGINE_CORE_API F_World : public A_WorldContext
    {
    public:
        friend class F_WorldManager;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(F_World);
        
    public:
        static F_Name GetCookTaskTag();
        
    public:
        static TW<F_World> _Main;
        static ABYTEK_FORCE_INLINE const auto& GetMain() noexcept
        {
            return _Main;
        }
        
    public:
        static TS<F_World> Create(const F_WorldConfig& Config);

    private:
        E_WorldFlag _Flags = E_WorldFlag::NONE;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TS<F_CookProfile> _CookProfile;
#endif
        
        TS<F_SerializableEnvironment> _OwnedSerializableEnvironment;
        
        TU<F_WorldSubsystemContainer> _SubsystemContainer;
        
        TF_Set<TS<F_Level>> _PersistentLevels;
        B8 _CanModifyPersistentLevels = false;
        TF_Set<TS<F_Level>> _Levels;
        
        TS<F_WorldBusiness> _PersistentBusiness;
        TS<F_WorldBusiness> _SessionBusiness;
        
        B8 _RequestedToStop = false;
        TF_Queue<TF_Function<void()>> _StopCallbacks;
        
        B8 _RequestedToTravel = false;
        TF_Vector<TS<F_Level>> _LevelsToTravel;
        enum class E_TravelMode : U8
        {
            NONE,
            NORMAL,
            STOP,
            START
        };
        E_TravelMode _TravelMode = E_TravelMode::NONE;
        
    public:
        F_WorldEvents Events;
        
    public:
        ABYTEK_FORCE_INLINE auto GetFlags() const noexcept
        {
            return _Flags;
        }
        ABYTEK_FORCE_INLINE auto HasFlags(E_WorldFlag Value) const noexcept
        {
            return FlagHas(_Flags, Value);
        }
        ABYTEK_FORCE_INLINE auto HasFlagsAny(E_WorldFlag Value) const noexcept
        {
            return FlagHasAny(_Flags, Value);
        }
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        ABYTEK_FORCE_INLINE const auto& GetCookProfile() const noexcept
        {
            return _CookProfile;
        }
#endif
        
        ABYTEK_FORCE_INLINE auto GetSubsystemContainer() const noexcept
        {
            return _SubsystemContainer.Weak();
        }
        
        ABYTEK_FORCE_INLINE const auto& GetPersistentLevels() const noexcept
        {
            return _PersistentLevels;
        }
        ABYTEK_FORCE_INLINE auto CanModifyPersistentLevels() const noexcept
        {
            return _CanModifyPersistentLevels;
        }
        ABYTEK_FORCE_INLINE const auto& GetLevels() const noexcept
        {
            return _Levels;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetPersistentBusiness() const noexcept
        {
            return _PersistentBusiness;
        }
        ABYTEK_FORCE_INLINE const auto& GetSessionBusiness() const noexcept
        {
            return _SessionBusiness;
        }
        
    public:
        F_World(const F_SerializableObjectInitParams& InitParams);
        ~F_World() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    public:
        void _ImmmediateStart(const F_WorldConfig& Config);
        void _ImmediateStop();
        
    public:
        void Stop(TF_Function<void()>&& Callback = {});
        
    public:
        B8 HasLevel(const TW_Valid<F_Level>& Level);
        void AddLevel(const TS_Valid<F_Level>& Level);
        void RemoveLevel(const TW_Valid<F_Level>& Level);
        void RemoveAllLevels();
        
    public:
        B8 HasPersistentLevel(const TW_Valid<F_Level>& Level);
        void AddPersistentLevel(const TS_Valid<F_Level>& Level);
        void RemovePersistentLevel(const TW_Valid<F_Level>& Level);
       
    private:
        void _ImmediateTravel();
        
    public:
        void Travel(const TF_Vector<TS<F_Level>>& Levels);
        void Travel(const TS<F_Level>& Level);
        void Travel(const F_Name& LevelPath, const TF_ReflectionTypeHandle<F_Level>& LevelType = TF_ReflectionTypeHandle<F_Level>(F_ReflectionContext::GetGlobal()));
    
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    private:
        void _Cook();
        void _BuildConfigDirectory();
#endif
        
    public:
        static B8 IsCookMode()
        {
            return GetMain()->HasFlags(E_WorldFlag::COOK_MODE);
        }
    };
}

#define ABYTEK_DECLARE_STATIC_WORLD_SUBOBJECT(...) \
            static Abytek::F_Name GetWorldSubobjectName();

#define ABYTEK_DEFINE_STATIC_WORLD_SUBOBJECT(...) \
            Abytek::F_Name __VA_ARGS__ :: GetWorldSubobjectName() { return Abytek::ToText(Abytek::TypeFullName<__VA_ARGS__>()); }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
#define ABYTEK_CHECK_COOK() ABYTEK_CHECK_TASK_TAG(Abytek::F_World::GetCookTaskTag())
#endif