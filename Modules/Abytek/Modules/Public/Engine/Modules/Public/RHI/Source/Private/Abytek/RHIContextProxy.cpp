#include "Abytek/RHIContextProxy.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIProcess.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIContextProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIContextProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIContextProxy>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHIContextProxy::Build(const F_RHIContextProxyBuildParams& BuildParams)
    {
        A_RAObject::BuildMinimal(); 
        _Context = BuildParams.Context;
        _Device = _Context->GetDevice();
    }
    void A_RHIContextProxy::Release()
    {        
        _IsFirstExecution = true;
        _Device = {};
        _Context = {};
        A_RAObject::Release();
    }

    void A_RHIContextProxy::BeginExecution(const TW_Valid<A_RHIProcess>& Process)
    {
        _ExecutionLock.Lock();
        _CurrentProcess = Process;
            
        if (IsFirstExecution())
        {
            FirstExecution();
        }
    }
    void A_RHIContextProxy::EndExecution()
    {
        _CurrentProcess = {};
        _ExecutionLock.Unlock();
    }

    void A_RHIContextProxy::FirstExecution()
    {
    }
}
