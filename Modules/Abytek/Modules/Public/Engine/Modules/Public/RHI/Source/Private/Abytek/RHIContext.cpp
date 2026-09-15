#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIContextProxy.hpp"
#include "Abytek/RHIProcess.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIContext.hpp"
#include "Abytek/RHITemplateRuntimeDatabase.hpp"
#include "Abytek/RHITransientUploadBufferManager.hpp"
#include "Abytek/RHITransientReadbackBufferManager.hpp"
#include "Abytek/RHIConstantDataManager.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIContext)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIContext>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHIContext::Build(const F_RHIContextBuildParams& BuildParams)
    {
        A_RAObject::BuildMinimal();
            
        _Device = BuildParams.Device; 
            
        _RunFirstProcess();
    }
    void A_RHIContext::Release()
    {        
        // Last submit to finalize + synchronize
        {
            F_RHIProcessBuildParams ProcessBuildParams;
            ProcessBuildParams.Contexts = { ABYTEK_WTHIS() };
            auto Process = RACreateAndBuildShared<A_RHIProcess>(ProcessBuildParams);
            
            HighLevelDeinitialize();
            
            ABYTEK_AWAIT Process->Flush();
        }
            
        FinalizeRelease();
            
        _Device = {};
        _IsFirstCompile = true;
            
        A_RAObject::Release();
    }

    void A_RHIContext::_RunFirstProcess()
    {
        F_RHIProcessBuildParams ProcessBuildParams;
        ProcessBuildParams.Contexts = { ABYTEK_WTHIS() };
        auto Process = RACreateAndBuildShared<A_RHIProcess>(ProcessBuildParams);
            
        ABYTEK_AWAIT Process->Flush();
    }

    void A_RHIContext::FirstCompile()
    {
        F_RHIContextProxyBuildParams ProxyBuildParams;
        ProxyBuildParams.Context = ABYTEK_WTHIS();
        _Proxy = RACreateAndBuildShared<A_RHIContextProxy>(ProxyBuildParams);
            
        F_RHITemplateRuntimeDatabaseBuildParams TemplateRuntimeDatabaseBuildParams;
        TemplateRuntimeDatabaseBuildParams.Context = ABYTEK_WTHIS();
        _TemplateRuntimeDatabase = RACreateAndBuildShared<A_RHITemplateRuntimeDatabase>(TemplateRuntimeDatabaseBuildParams);
    }
    void A_RHIContext::FinalizeRelease()
    {
        _TemplateRuntimeDatabase = {};
        _Proxy = {};
    }

    void A_RHIContext::HighLevelInitialize()
    {
        {
            F_RHITransientUploadBufferManagerBuildParams UploadBufferManagerBuildParams;
            UploadBufferManagerBuildParams.Context = ABYTEK_WTHIS();
            _TransientUploadBufferManager = RACreateAndBuildShared<F_RHITransientUploadBufferManager>(UploadBufferManagerBuildParams);
        }
        {
            F_RHITransientReadbackBufferManagerBuildParams ReadbackBufferManagerBuildParams;
            ReadbackBufferManagerBuildParams.Context = ABYTEK_WTHIS();
            _TransientReadbackBufferManager = RACreateAndBuildShared<F_RHITransientReadbackBufferManager>(ReadbackBufferManagerBuildParams);
        }
        {
            F_RHIConstantDataManagerBuildParams ConstantDataManagerBuildParams;
            ConstantDataManagerBuildParams.Context = ABYTEK_WTHIS();
            _ConstantDataManager = RACreateAndBuildShared<F_RHIConstantDataManager>(ConstantDataManagerBuildParams);
        }
    }
    void A_RHIContext::HighLevelDeinitialize()
    {
        _ConstantDataManager = {};
        _TransientReadbackBufferManager = {};
        _TransientUploadBufferManager = {};
    }

    void A_RHIContext::BeginCompile(const TW_Valid<A_RHIProcess>& Process)
    {
        _CompileLock.Lock();
        _CurrentProcess = Process;
            
        if (IsFirstCompile())
        {
            FirstCompile();
            HighLevelInitialize();
        }
    }
    void A_RHIContext::EndCompile()
    {
        _IsFirstCompile = false;
        _CurrentProcess = {};
        _CompileLock.Unlock();
    }
}
