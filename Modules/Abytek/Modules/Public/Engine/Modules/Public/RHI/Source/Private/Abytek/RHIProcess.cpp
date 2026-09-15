#include "Abytek/RHIProcess.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIProcess.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIContextProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIProcess)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIProcess>()();
#endif 
        } 
    ABYTEK_RA_END_OBJECT()
    AU32 A_RHIProcess::_StaticCounter = 0;
    void A_RHIProcess::Build(const F_RHIProcessBuildParams& BuildParams)
    {
        A_RAObject::BuildMinimal();
        
        _Counter = _StaticCounter.fetch_add(1, boost::memory_order_release);
        _Contexts = BuildParams.Contexts;
        
        _EnableDriverDebugger = F_RHISubsystem::GetInstance()->GetConsoleVariable_EnableDriverDebugger()->GetValue();
            
        _Arena = TU_Unmanaged<F_Arena>()(Sz(0x10000));
            
        _CompilePromise = TS_Unmanaged<F_TaskPromise>()();
        _MainPromise = TS_Unmanaged<F_TaskPromise>()();
        _FinalPromise = TS_Unmanaged<F_TaskPromise>()();
#ifdef ABYTEK_DEBUG_INFO
        _CompilePromise->SetDebugName(ABYTEK_DEBUG_NAME("RHIProcess::CompilePromise"));
        _MainPromise->SetDebugName(ABYTEK_DEBUG_NAME("RHIProcess::MainPromise"));
        _FinalPromise->SetDebugName(ABYTEK_DEBUG_NAME("RHIProcess::FinalPromise"));
#endif
            
        _Stage.store(E_RHIProcessStage::COMPILE, boost::memory_order_release);
#ifdef ABYTEK_DEBUG_INFO
        _ZoneName = ABYTEK_TEXT("BeginCompile::Before");
#endif
        for (const auto& Context : _Contexts)
        {
            Context->BeginCompile(ABYTEK_WTHIS());
        }
#ifdef ABYTEK_DEBUG_INFO
        _ZoneName = ABYTEK_TEXT("BeginCompile::After");
#endif
        
        _UploadSubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(ABYTEK_WTHIS());
        _UploadCopySubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(ABYTEK_WTHIS());
        _ReadbackSubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(ABYTEK_WTHIS());
        _ReadbackCopySubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(ABYTEK_WTHIS());
#ifdef ABYTEK_DEBUG_INFO
        _UploadSubmissionList->SetDebugName(ABYTEK_DEBUG_NAME("RHIUpload"));   
        _UploadCopySubmissionList->SetDebugName(ABYTEK_DEBUG_NAME("RHIUploadCopy"));   
        _ReadbackSubmissionList->SetDebugName(ABYTEK_DEBUG_NAME("RHIReadback"));   
        _ReadbackCopySubmissionList->SetDebugName(ABYTEK_DEBUG_NAME("RHIReadbackCopy"));   
#endif
    }
    void A_RHIProcess::Release()
    {        
        _IsFirstFlush = true;
        
#ifdef ABYTEK_DEBUG_INFO
        _ZoneName = ABYTEK_TEXT("None");
#endif
        _Stage.store(E_RHIProcessStage::NONE, boost::memory_order_release);
        
        _CurrentSection_EndViewportIndex = 0;
        _CurrentSection_BeginViewportIndex = 0;
        
        _CurrentSection_EndRootSubmissionItemIndex = 0;
        _CurrentSection_BeginRootSubmissionItemIndex = 0;
        
        _FinalPromise = {};
        _MainPromise = {};
        _CompilePromise = {};
        _Arena = {};
        ABYTEK_ENGINE_RHI_ASSERT(_PostCompileCommands.GetSize() == 0) << "PostCompileCommands was not flushed";
        ABYTEK_ENGINE_RHI_ASSERT(_FlushCompileDataCommands.GetSize() == 0) << "FlushCompileDataCommands was not flushed";
        ABYTEK_ENGINE_RHI_ASSERT(_PreExecuteCommands.GetSize() == 0) << "PreExecuteCommands was not flushed";
        ABYTEK_ENGINE_RHI_ASSERT(_PostExecuteCommands.GetSize() == 0) << "PostExecuteCommands was not flushed";
        ABYTEK_ENGINE_RHI_ASSERT(_FlushExecuteDataCommands.GetSize() == 0) << "PostExecuteCommands was not flushed";
        ABYTEK_ENGINE_RHI_ASSERT(_ContextProxies.size() == 0) << "ContextProxies was not flushed";
        ABYTEK_ENGINE_RHI_ASSERT(_Contexts.size() == 0) << "Contexts was not flushed";
        _EnableDriverDebugger = false;
        _Counter = ~U32(0);
        A_RAObject::Release();
    }

    void A_RHIProcess::PreCompile(E_RHIProcessFlushFlag Flags)
    {
    }
    void A_RHIProcess::Compile(E_RHIProcessFlushFlag Flags)
    {
    }
    void A_RHIProcess::PostCompile(E_RHIProcessFlushFlag Flags)
    {
        _CurrentSection_EndRootSubmissionItemIndex = _CurrentSection_BeginRootSubmissionItemIndex;
        _CurrentSection_EndViewportIndex = _CurrentSection_BeginViewportIndex;
    }

    void A_RHIProcess::CleanCompile()
    { 
#ifdef ABYTEK_DEBUG_INFO
        _ZoneName = ABYTEK_TEXT("FlushPostCompileCommands");
#endif
        _FlushPostCompileCommands();
#ifdef ABYTEK_DEBUG_INFO
        _ZoneName = ABYTEK_TEXT("FlushCompileData");
#endif
        _FlushCompileData();  
#ifdef ABYTEK_DEBUG_INFO
        _ZoneName = ABYTEK_TEXT("Release_RootSubmissionItems::Before");
#endif
        _RootSubmissionItems = {};
#ifdef ABYTEK_DEBUG_INFO
        _ZoneName = ABYTEK_TEXT("Release_RootSubmissionItems::After");
#endif
        
        _Viewports = {};
        
        _ReadbackCopySubmissionList = {};
        _ReadbackSubmissionList = {};
        _UploadCopySubmissionList = {};
        _UploadSubmissionList = {};
    }
    void A_RHIProcess::_FlushPostCompileCommands()
    {
        F_RHIProcessPostCompileCommand Command;
        while (_PostCompileCommands.TryPop(Command))
        {
            Command();
        }
    }
    void A_RHIProcess::_FlushCompileData()
    {
        F_RHIProcessFlushCompileDataCommand Command;
        while (_FlushCompileDataCommands.TryPop(Command))
        {
            Command();
        }
    }
    void A_RHIProcess::_FlushPreExecuteCommands()
    {
        F_RHIProcessPreExecuteCommand Command;
        while (_PreExecuteCommands.TryPop(Command))
        {
            Command();
        }
    }
    void A_RHIProcess::_FlushPostExecuteCommands()
    {
        F_RHIProcessPostExecuteCommand Command;
        while (_PostExecuteCommands.TryPop(Command))
        {
            Command();
        }
    }
    void A_RHIProcess::_FlushExecuteData()
    {
        F_RHIProcessFlushExecuteDataCommand Command;
        while (_FlushExecuteDataCommands.TryPop(Command))
        {
            Command();
        }
    }
    void A_RHIProcess::CleanExecute()
    {
        _FlushPostExecuteCommands();
        _FlushExecuteData();
    }
    void A_RHIProcess::_FlushProcessData()
    {
        F_RHIProcessFlushProcessDataCommand Command;
        while (_FlushProcessDataCommands.TryPop(Command))
        {
            Command();
        }
    }
    void A_RHIProcess::Execute()
    {
    }
    void A_RHIProcess::BeginLateExecute()
    {
    }
    void A_RHIProcess::EndLateExecute()
    {
    }

    TS_Unmanaged<F_TaskPromise> A_RHIProcess::Flush(E_RHIProcessFlushFlag Flags)
    {
        return H_TaskUtilities::Schedule(
            [RAThis = ABYTEK_STHIS(), this, Flags]()
            {
                ABYTEK_PROFILER_EVENT_NAMED("A_RHIProcessV2::Flush");
                
#ifdef ABYTEK_DEBUG_INFO
                _ZoneName = ABYTEK_TEXT("LockProcess::Before");
#endif
                 
                _SectionLock.Lock(E_TaskPriority::MEDIUM);
                H_TaskUtilities::ChangePriority(E_TaskPriority::EXTREME);
                
#ifdef ABYTEK_DEBUG_INFO
                _ZoneName = ABYTEK_TEXT("LockProcess::After");
#endif
                
#ifdef ABYTEK_DEBUG_INFO
                _ZoneName = ABYTEK_TEXT("PreCompile");
#endif
                PreCompile(Flags);
                H_TaskUtilities::Switch();
#ifdef ABYTEK_DEBUG_INFO
                _ZoneName = ABYTEK_TEXT("Compile");
#endif
                Compile(Flags);
                H_TaskUtilities::Switch();
#ifdef ABYTEK_DEBUG_INFO
                _ZoneName = ABYTEK_TEXT("PostCompile");
#endif
                PostCompile(Flags);
                H_TaskUtilities::Switch();
                
                if (FlagHas(Flags, E_RHIProcessFlushFlag::EXECUTE))
                {
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("BeginExecution::Before");
#endif
                    for (const auto& Context : _Contexts)
                    {
                        _ContextProxies.push_back(Context->GetProxy());
                    }
                    for (const auto& ContextProxy : _ContextProxies)
                    {
                        ContextProxy->BeginExecution(ABYTEK_WTHIS());
                    }  
                    H_TaskUtilities::Switch();
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("BeginExecution::After");
#endif
                    
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("CleanCompile::Before");
#endif
                    CleanCompile();
                    H_TaskUtilities::Switch();
                    _Stage.store(E_RHIProcessStage::EXECUTE, boost::memory_order_release);
                    
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("CleanCompile::After");
#endif
                    _CompilePromise->DecreaseCounter();
                    
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("EndCompile::Before");
#endif
                    for (const auto& Context : _Contexts)
                    {
                        Context->EndCompile();
                    }
                    H_TaskUtilities::Switch();
                    _Contexts = {};
                    _Viewports = {};
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("EndCompile::After");
#endif
                    
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("FlushPreExecuteCommands");
#endif
                    _FlushPreExecuteCommands();
                    H_TaskUtilities::Switch();
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("Execute");
#endif
                    Execute();
                    H_TaskUtilities::Switch();
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("BeginLateExecute");
#endif
                    BeginLateExecute();
                    H_TaskUtilities::Switch();
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("CleanExecute");
#endif
                    CleanExecute();
                    H_TaskUtilities::Switch();
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("FlushProcessData");
#endif
                    _FlushProcessData();
                    H_TaskUtilities::Switch();
                    
                    _Arena->Reset();
                    H_TaskUtilities::Switch();
                    
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("EndExecution::Before");
#endif
                    for (const auto& ContextProxy : _ContextProxies)
                    {
                        ContextProxy->EndExecution();
                    }
                    H_TaskUtilities::Switch();
                    _ContextProxies = {};
                    H_TaskUtilities::Switch();
                    _Stage.store(E_RHIProcessStage::END, boost::memory_order_release);
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("EndExecution::After");
#endif
                    
                    _MainPromise->DecreaseCounter();
                     
#ifdef ABYTEK_DEBUG_INFO
                    _ZoneName = ABYTEK_TEXT("EndLateExecute");
#endif
                    EndLateExecute();
                    H_TaskUtilities::Switch();
                }
                
                _IsFirstFlush = false;
                
                _SectionLock.Unlock();
#ifdef ABYTEK_DEBUG_INFO
                _ZoneName = ABYTEK_TEXT("None");
#endif
            },
            E_TaskPriority::EXTREME,
            ABYTEK_NAME("A_RHIProcess::Flush"),
            _FinalPromise
        );
    }

    void A_RHIProcess::OnAddFrontSubmissionList(const TS_Valid<A_RHISubmissionList>& SubmissionList)
    {
        TF_ScopeLock<F_SpinLock> _(_QueueLock);
        ++_CurrentSection_EndRootSubmissionItemIndex;
        _RootSubmissionItems.insert(_RootSubmissionItems.begin() + _CurrentSection_BeginRootSubmissionItemIndex, SubmissionList);
    }
    void A_RHIProcess::OnAddBackSubmissionItem(const TS_Valid<A_RHISubmissionItem>& SubmissionItem)
    {
        TF_ScopeLock<F_SpinLock> _(_QueueLock);
        ++_CurrentSection_EndRootSubmissionItemIndex;
        _RootSubmissionItems.push_back(SubmissionItem);
    }

    void A_RHIProcess::AddViewport(const TS_Valid<A_RHIViewport>& Viewport)
    {
        TF_ScopeLock<F_SpinLock> _(_QueueLock);
        if (
            std::find(
                _Viewports.begin(),    
                _Viewports.end(),
                Viewport
            )    
            == _Viewports.end()
        )
        {
            _Viewports.push_back(Viewport);
            ++_CurrentSection_EndViewportIndex;
        }
    }
}
