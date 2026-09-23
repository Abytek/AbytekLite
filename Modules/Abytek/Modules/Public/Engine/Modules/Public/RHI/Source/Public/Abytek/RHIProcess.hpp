#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHISubmissionItemContainerInterface.hpp"
#include "Abytek/RHIProcessFlushFlag.hpp"
#include "Abytek/RHICaptureEventState.hpp"


namespace Abytek
{
    class A_RHIContext;
    class A_RHIContextProxy;
    class A_RHISubmissionItem;
    
    using F_RHIProcessPostCompileCommand = TF_Function<void()>;
    using F_RHIProcessFlushCompileDataCommand = TF_Function<void()>;
    using F_RHIProcessPreExecuteCommand = TF_Function<void()>;
    using F_RHIProcessPostExecuteCommand = TF_Function<void()>;
    using F_RHIProcessFlushExecuteDataCommand = TF_Function<void()>;
    using F_RHIProcessFlushProcessDataCommand = TF_Function<void()>;
    
    enum class E_RHIProcessStage : U8
    {
        NONE,
        COMPILE,
        EXECUTE,
        END
    };
    
    struct F_RHIProcessBuildParams
    {
        TF_Vector<TW<A_RHIContext>> Contexts;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIProcess : public A_RAObject, public I_RHISubmissionItemContainer
    {
    private:
        U32 _Counter = ~U32(0);
        TF_Vector<TW<A_RHIContext>> _Contexts;
        
        B8 _EnableDriverDebugger = false;
        B8 _EnablePassSorting = false;
        
        TF_Vector<TS<A_RHIContextProxy>> _ContextProxies;
        
        mutable F_SpinLock _QueueLock;
        
        TF_Vector<TS<A_RHISubmissionItem>> _RootSubmissionItems;
        U32 _CurrentSection_BeginRootSubmissionItemIndex = 0;
        U32 _CurrentSection_EndRootSubmissionItemIndex = 0;
        
        TF_ConcurrentQueue<F_RHIProcessPostCompileCommand> _PostCompileCommands;
        TF_ConcurrentQueue<F_RHIProcessFlushCompileDataCommand> _FlushCompileDataCommands;
        TF_ConcurrentQueue<F_RHIProcessPreExecuteCommand> _PreExecuteCommands;
        TF_ConcurrentQueue<F_RHIProcessPreExecuteCommand> _PostExecuteCommands;
        TF_ConcurrentQueue<F_RHIProcessFlushExecuteDataCommand> _FlushExecuteDataCommands;
        TF_ConcurrentQueue<F_RHIProcessFlushProcessDataCommand> _FlushProcessDataCommands;
        
        TU_Unmanaged<F_Arena> _Arena;
        
        mutable F_YieldSpinLock _SectionLock;
        B8 _IsFirstFlush = true;
        
        TS_Unmanaged<F_TaskPromise> _CompilePromise;
        TS_Unmanaged<F_TaskPromise> _MainPromise;
        TS_Unmanaged<F_TaskPromise> _FinalPromise;
        
        boost::atomic<E_RHIProcessStage> _Stage;
        
#ifdef ABYTEK_DEBUG_INFO
        F_Text _ZoneName;
#endif
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        F_RHICaptureEventState _CaptureEventState;
#endif
        
    public:
        ABYTEK_FORCE_INLINE auto GetCounter() const noexcept
        {
            return _Counter;
        }
        ABYTEK_FORCE_INLINE const auto& GetContexts() const noexcept
        {
            return _Contexts;
        }
        
        ABYTEK_FORCE_INLINE auto GetEnableDriverDebugger() const noexcept
        {
            return _EnableDriverDebugger;
        }
        ABYTEK_FORCE_INLINE auto GetEnablePassSorting() const noexcept
        {
            return _EnablePassSorting;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetContextProxies() const noexcept
        {
            return _ContextProxies;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetRootSubmissionItems() const noexcept
        {
            return _RootSubmissionItems;
        }
        ABYTEK_FORCE_INLINE auto GetCurrentSection_BeginRootSubmissionItemIndex() const noexcept
        {
            return _CurrentSection_BeginRootSubmissionItemIndex;
        }
        ABYTEK_FORCE_INLINE auto GetCurrentSection_EndRootSubmissionItemIndex() const noexcept
        {
            return _CurrentSection_EndRootSubmissionItemIndex;
        }
        
        ABYTEK_FORCE_INLINE auto GetArena() const noexcept
        {
            return _Arena.Weak();
        }
        
        ABYTEK_FORCE_INLINE auto IsFirstFlush() const noexcept
        {
            return _IsFirstFlush;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetCompilePromise() const noexcept
        {
            return _CompilePromise;
        }
        ABYTEK_FORCE_INLINE const auto& GetMainPromise() const noexcept
        {
            return _MainPromise;
        }
        ABYTEK_FORCE_INLINE const auto& GetFinalPromise() const noexcept
        {
            return _FinalPromise;
        }
        
        ABYTEK_FORCE_INLINE auto GetStage() const noexcept
        {
            return _Stage.load(boost::memory_order_acquire);
        }
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        ABYTEK_FORCE_INLINE auto& GetCaptureEventState() noexcept
        {
            return _CaptureEventState;
        }
        ABYTEK_FORCE_INLINE const auto& GetCaptureEventState() const noexcept
        {
            return _CaptureEventState;
        }
        void SetCaptureEventState(const F_RHICaptureEventState& Value) noexcept
        {
            _CaptureEventState = Value;
        }
#endif
        
    public:
        static AU32 _StaticCounter;
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIProcess);
        virtual void Build(const F_RHIProcessBuildParams& BuildParams);
        virtual void Release() override;
    
    public:
        template<typename __F_Command>
        void EnqueuePostCompileCommand(__F_Command&& Command)
        {
            _PostCompileCommands.Push(ABYTEK_FORWARD(Command));
        }
        template<typename __F_Command>
        void EnqueuePreExecuteCommand(__F_Command&& Command)
        {
            _PreExecuteCommands.Push(ABYTEK_FORWARD(Command));
        }
        template<typename __F_Command>
        void EnqueuePostExecuteCommand(__F_Command&& Command)
        {
            _PostExecuteCommands.Push(ABYTEK_FORWARD(Command));
        }
        
    protected:
        virtual void PreCompile(E_RHIProcessFlushFlag Flags);
        virtual void Compile(E_RHIProcessFlushFlag Flags);
        virtual void PostCompile(E_RHIProcessFlushFlag Flags);
        virtual void CleanCompile();
        virtual void Execute();
        virtual void CleanExecute();
        virtual void BeginLateExecute();
        virtual void EndLateExecute();
        
    private:
        void _FlushPostCompileCommands();
        void _FlushCompileData();
        void _FlushPreExecuteCommands();
        void _FlushPostExecuteCommands();
        void _FlushExecuteData();
        void _FlushProcessData();
    
    public:
        virtual TS_Unmanaged<F_TaskPromise> Flush(E_RHIProcessFlushFlag Flags = E_RHIProcessFlushFlag::DEFAULT);
        
    public:
        template<class __F_CompileData>
        __F_CompileData* NewCompileData(U32& OutProcessCounter)
        {
            auto Result = GetArena()->New<__F_CompileData>();
            OutProcessCounter = _Counter;
            _FlushCompileDataCommands.Push(
                [Result]
                {
                    Result->~__F_CompileData();
                }
            );
            return Result;
        }
        template<class __F_ExecuteData>
        __F_ExecuteData* NewExecuteData(U32& OutProcessCounter)
        {
            auto Result = GetArena()->New<__F_ExecuteData>();
            OutProcessCounter = _Counter;
            _FlushExecuteDataCommands.Push(
                [Result]
                {
                    Result->~__F_ExecuteData();
                }
            );
            return Result;
        }
        template<class __F_ProcessData>
        __F_ProcessData* NewProcessData(U32& OutProcessCounter)
        {
            auto Result = GetArena()->New<__F_ProcessData>();
            OutProcessCounter = _Counter;
            _FlushProcessDataCommands.Push(
                [Result]
                {
                    Result->~__F_ProcessData();
                }
            );
            return Result;
        }
        
    protected:
        void OnAddFrontSubmissionList(const TS<A_RHISubmissionList>& SubmissionList) override;
        void OnAddBackSubmissionItem(const TS<A_RHISubmissionItem>& SubmissionItem) override;
        
    protected:
        TS<A_RHISubmissionList> OnAddChild(E_RHISubmissionListOrder Order, const F_DebugName& DebugName) override;
        
#ifdef ABYTEK_DEBUG_INFO
    public:
        void SetDebugName(const F_DebugName& Value) noexcept override;
#endif
    };
}
