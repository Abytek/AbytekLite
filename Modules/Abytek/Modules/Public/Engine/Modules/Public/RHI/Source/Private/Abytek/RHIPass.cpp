#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIProcess.hpp"


namespace Abytek
{
    void A_RHIPassProxy::Release()
    {
        _PassPosition = E_RHIPassPosition::NONE;
        _PassClass = E_RHIPassClass::NONE;
        _GPUWorkClass = E_RHIGPUWorkClass::NONE;
        _Process = {};
        _ContextProxy = {}; 
        A_RAObject::Release();
    }
    void A_RHIPassProxy::BuildPassProxy(const TW_Valid<A_RHIPass>& Pass)
    {
        A_RAObject::BuildMinimal();
        _ContextProxy = Pass->GetContext()->GetProxy().Weak();
        _Process = Pass->GetProcess();
        _GPUWorkClass = Pass->GetGPUWorkClass();
        _PassClass = Pass->GetPassClass();
        _PassPosition = Pass->GetPassPosition();
    }

    void A_RHIPass::Build(const F_RHIPassBuildParams& BuildParams)
    {
        BuildSubmissionItem();
        _Context = BuildParams.Context;
        _GPUWorkClass = BuildParams.GPUWorkClass;
    }
    void A_RHIPass::Release()
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
        if (auto Process = ShareObject(GetProcess()))
        {
            ABYTEK_ENGINE_RHI_ASSERT(Process->GetStage() == E_RHIProcessStage::COMPILE) << "Invalid moment to release submission item (too late, must be inside the compile stage)";
        }
#endif
        _GPUWorkClass = E_RHIGPUWorkClass::NONE;
        _Context = {};
        A_RHISubmissionItem::Release();
    }

    E_RHIPassClass A_RHIPass::GetPassClass() const
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "Not implemented";
        return E_RHIPassClass::NONE;
    }
    E_RHIPassPosition A_RHIPass::GetPassPosition() const
    {
        return E_RHIPassPosition::DEFAULT;
    }

    TS_Valid<A_RHIPassProxy> A_RHIPass::CreateProxy()
    {
        return {};
    }

    TW_Valid<A_RHIProcess> A_RHIPass::GetProcess() const
    {
        return _Context->GetCurrentProcess();
    }
}
