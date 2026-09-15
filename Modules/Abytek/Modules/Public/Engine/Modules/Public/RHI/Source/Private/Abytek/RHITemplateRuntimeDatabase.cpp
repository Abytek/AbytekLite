#include "Abytek/RHITemplateRuntimeDatabase.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHITemplateRuntimeDatabase.hpp"
#include "Abytek/RHITemplate.hpp"
#include "Abytek/RHITemplateRuntime.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHITemplateRuntimeDatabase)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHITemplateRuntimeDatabase>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT();
    void A_RHITemplateRuntimeDatabase::Build(const F_RHITemplateRuntimeDatabaseBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
    }
    void A_RHITemplateRuntimeDatabase::Release()
    {
        _TemplateRuntimes = {};
        A_RHIContextChild::Release();
    }

    void A_RHITemplateRuntimeDatabase::_TrackTemplateRuntime(const TW_Valid<A_RHITemplateRuntime>& TemplateRuntime)
    {
        // No need to lock because we are always in lock due to GetOrActivateRuntime 
        ABYTEK_ENGINE_RHI_ASSERT(_SpinLock.ProducerTryLock() == false) << "Requires to be already in critical session";
        ABYTEK_ENGINE_RHI_ASSERT(_TemplateRuntimes.find(TemplateRuntime->GetHashCode()) == _TemplateRuntimes.end());
        _TemplateRuntimes.insert({ TemplateRuntime->GetHashCode(), TemplateRuntime });
    }
    void A_RHITemplateRuntimeDatabase::_UntrackTemplateRuntime(const TW_Valid<A_RHITemplateRuntime>& TemplateRuntime)
    {
        TF_ScopeLock<F_SpinLock> _(_SpinLock);
        ABYTEK_ENGINE_RHI_ASSERT(_TemplateRuntimes.find(TemplateRuntime->GetHashCode()) != _TemplateRuntimes.end());
        _TemplateRuntimes.erase(_TemplateRuntimes.find(TemplateRuntime->GetHashCode()));
    }

    TS_Valid<A_RHITemplateRuntime> A_RHITemplateRuntimeDatabase::GetOrActivateRuntime(
        const TS_Valid<A_RHITemplate>& Template
    )
    {
        TS<A_RHITemplateRuntime> TemplateRuntime;
        B8 NeedToActivate = false;
        {
            TF_ScopeLock<F_SpinLock> _(_SpinLock);
            auto It = _TemplateRuntimes.find(Template->GetHashCode());
            if (It != _TemplateRuntimes.end())
            {
                TemplateRuntime = ShareObject(It->second);
            }
            NeedToActivate = TemplateRuntime.Q_IsNull();
            if (NeedToActivate)
            {
                TemplateRuntime = Template->CreateAndBuildRuntime(GetContext());
            }
        }
        if (NeedToActivate)
        {
            TemplateRuntime->FinalizeActivationAndUnlock();
        }
        else
        {
            TemplateRuntime->WaitForActivation();
        }
        return ABYTEK_MOVE(TemplateRuntime);
    }
    TS<A_RHITemplateRuntime> A_RHITemplateRuntimeDatabase::Find(F_RHITemplateHashCode TemplateHashCode)
    {
        TF_ScopeLock<F_SpinLock> _(_SpinLock);
        auto It = _TemplateRuntimes.find(TemplateHashCode);
        if (It != _TemplateRuntimes.end())
        {
            return ShareObject(It->second);
        }
        return {};
    }
}
