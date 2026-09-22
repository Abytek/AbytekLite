#pragma once

#include "Abytek/ProgramUtilities/Program.hpp"
#include "Abytek/Frame/FrameCommon.hpp"


namespace Abytek
{
    struct F_HighLevelUpdateRange;
    struct F_PrimaryUpdateRange;
    struct F_StartupUpdateRange;
    struct F_PostStartupUpdateRange;
    struct F_TickUpdateRange;
    struct F_PostTickUpdateRange;
    struct F_PreShutdownUpdateRange;
    struct F_ShutdownUpdateRange;
    class F_FrameManager;
    class F_UpdateManager;
    class F_ApplicationMountManager;
    
    inline F_Name GetApplicationLowLevelTaskTag()
    {
        return ABYTEK_NAME("Abytek::ApplicationLowLevel");
    }
#define ABYTEK_CHECK_APPLICATION_LOW_LEVEL() ABYTEK_CHECK_TASK_TAG(Abytek::GetApplicationLowLevelTaskTag())
#define ABYTEK_CHECK_NOT_APPLICATION_LOW_LEVEL() ABYTEK_CHECK_NOT_TASK_TAG(Abytek::GetApplicationLowLevelTaskTag())
    
    namespace ApplicationDebugMemoryLeakByFrame
    {
        struct F_Params
        {
            F_MemoryAnalyzedState* BeginMemoryAnalyzedState = nullptr;
            F_MemoryAnalyzedState* EndMemoryAnalyzedState = nullptr;
            F_MemoryAnalyzedState* DifferentMemoryAnalyzedState = nullptr;
            
            TF_Vector<A_Object*>* BeginTrackedObjects = nullptr;
            TF_Vector<A_Object*>* EndTrackedObjects = nullptr;
            TF_Vector<A_Object*>* DifferentTrackedObjects = nullptr;
        };
    };
    namespace ApplicationEvents
    {
        using F_OnDebugMemoryLeakByFrame = TF_Event<void(const ApplicationDebugMemoryLeakByFrame::F_Params& Params)>;
    }
    struct F_ApplicationEvents
    {
        ApplicationEvents::F_OnDebugMemoryLeakByFrame OnDebugMemoryLeakByFrame;
    };
    
    struct F_ApplicationBuildParams
    {
        F_Name Name;
        TF_Function<void()> ExternalInitFunction;
    };
    
    enum class E_ApplicationState : U16
    {
        NONE = 0x0,
        INITIALIZING = 0x1,
        LLA_INITIALIZING = 0x2,
        STARTUP = 0x4,
        TICKING = 0x8,
        SHUTDOWN = 0x10,
        LLA_RELEASING = 0x20,
        RELEASING = 0x40,
        CLOSED = 0x40,
        
        ALL_HIGH_LEVEL = STARTUP | TICKING | SHUTDOWN,
        ALL = INITIALIZING | LLA_INITIALIZING | ALL_HIGH_LEVEL | LLA_RELEASING | RELEASING | CLOSED
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_ApplicationState);
    
    class ABYTEK_ENGINE_CORE_API A_ApplicationCore : public A_Program
    {
        ABYTEK_BEGIN_REFLECTOR(A_Program)
        ABYTEK_END_REFLECTOR(A_ApplicationCore);
        
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_ApplicationCore);
        
    public:
        friend class F_EngineRuntime;
        
    public:
        static F_Name GetStartupUpdateFunctionName();
        static F_Name GetPostStartupUpdateFunctionName();
        static F_Name GetTickUpdateFunctionName();
        static F_Name GetPostTickUpdateFunctionName();
        static F_Name GetPreShutdownUpdateFunctionName();
        static F_Name GetShutdownUpdateFunctionName();
        
    public:
        static F_Text GetConfigFileName();
        void GatherConfigFilePaths(TF_Vector<F_Text>& FilePaths) override;

    private:
        F_Name _Name;
        TF_Function<void()> _ExternalInitFunction;
        B8 _ShouldShutdown = false;
        E_ApplicationState _State = E_ApplicationState::NONE;
        E_ApplicationState _NextState = E_ApplicationState::INITIALIZING;
        TU<F_ApplicationMountManager> _MountManager;
        TU<F_FrameManager> _FrameManager;
        TU<F_UpdateManager> _UpdateManager;
        TU<F_HighLevelUpdateRange> _HighLevelUpdateRange;
        TU<F_PrimaryUpdateRange> _PrimaryUpdateRange;
        TU<F_StartupUpdateRange> _StartupUpdateRange;
        TU<F_PostStartupUpdateRange> _PostStartupUpdateRange;
        TU<F_TickUpdateRange> _TickUpdateRange;
        TU<F_PostTickUpdateRange> _PostTickUpdateRange;
        TU<F_PreShutdownUpdateRange> _PreShutdownUpdateRange;
        TU<F_ShutdownUpdateRange> _ShutdownUpdateRange;
        AU32 _LLAInitCounter = 0;
        AU32 _LLAReleaseCounter = 0;
        
        TF_Vector<TF_ReflectionTypeHandle<A_SerializableObject>> _CDOTypes;

        TW<TF_ConsoleVariable<E_FramesInFlightMode>> _ConsoleVariable_FramesInFlightMode;
        
    public:
        F_ApplicationEvents Events;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }
        ABYTEK_FORCE_INLINE B8 ShouldShutdown() const noexcept
        {
            return _ShouldShutdown;
        }
        ABYTEK_FORCE_INLINE E_ApplicationState GetState() const noexcept
        {
            return _State;
        }
        ABYTEK_FORCE_INLINE U32 GetLLAInitCounter() const noexcept
        {
            return _LLAInitCounter.load(boost::memory_order_acquire);
        }
        ABYTEK_FORCE_INLINE U32 GetLLAReleaseCounter() const noexcept
        {
            return _LLAReleaseCounter.load(boost::memory_order_acquire);
        }
        
    protected:
        A_ApplicationCore(const F_ApplicationBuildParams& BuildParams);
        
    public:
        ~A_ApplicationCore() override;
        
    protected:
        TU<F_ModuleContainer> CreateModuleContainer() override;
        TU<F_SubsystemContainer> CreateSubsystemContainer() override;
        
    protected:
        void Reflect() override;
        void PostReflectModules() override;
        void PostCreateConsole() override;
        void Init() override;
        void Release() override;
        void PreDestroyConsole() override;
        void OnPreCleanUpModuleReflectionSessions() override;
        
    public:
        void UpdateFrame(E_ApplicationState State);
        void UpdateState();
        
    protected:
        virtual void Startup();
        virtual void PostStartup();
        virtual void Tick();
        virtual void PostTick();
        virtual void PreShutdown();
        virtual void Shutdown();
        
    protected:
        virtual void RegisterModules();
        
    public:
        virtual TF_ReflectionTypeHandle<F_Subsystem> GetBaseSubsystemType();
        
    public:
        U32 IncreaseLLAInitCounter(U32 Num = 1);
        U32 DecreaseLLAInitCounter(U32 Num = 1);
        U32 IncreaseLLAReleaseCounter(U32 Num = 1);
        U32 DecreaseLLAReleaseCounter(U32 Num = 1);
        
    public:
        void SignalShutdown();
        
    private:
        void _SetupCDOTypes();
        void _CleanUpCDOTypes();
    };
}
