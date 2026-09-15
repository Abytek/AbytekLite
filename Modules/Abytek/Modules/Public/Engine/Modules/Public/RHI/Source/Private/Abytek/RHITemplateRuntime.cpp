#include "Abytek/RHITemplateRuntime.hpp"
#include "Abytek/RHITemplateRuntimeDatabase.hpp"
#include "Abytek/RHIContext.hpp"


namespace Abytek
{
    auto A_RHITemplateRuntimeProxy::GetTemplateRuntime() const noexcept
    {
        return GetContextChild().FastCast<A_RHITemplateRuntime>();
    }

    void A_RHITemplateRuntimeProxy::LateBuildTemplateRuntime()
    {
        _Template = GetTemplateRuntime()->GetTemplate();
    }
    void A_RHITemplateRuntimeProxy::Release()
    {
        _Template = {};
        A_RHIContextChildProxy::Release();
    }

    TW_Valid<A_RHITemplateRuntimeDatabase> A_RHITemplateRuntime::GetDatabase() const noexcept
    {
        return GetContext()->GetTemplateRuntimeDatabase().Weak();
    }

    void A_RHITemplateRuntime::Build(const F_RHITemplateRuntimeBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        ABYTEK_ENGINE_RHI_ASSERT(BuildParams.Template) << "Invalid template";
        _Template = BuildParams.Template;
        GetDatabase()->_TrackTemplateRuntime(ABYTEK_WTHIS());
    }
    void A_RHITemplateRuntime::Release()
    {
        GetDatabase()->_UntrackTemplateRuntime(ABYTEK_WTHIS());
        _IsActivated.clear(boost::memory_order_release);
        _Template = {};
        A_RHIContextChild::Release();
    }

    void A_RHITemplateRuntime::FinalizeActivation()
    {
        GetProxy().FastCast<A_RHITemplateRuntimeProxy>()->LateBuildTemplateRuntime();
    }

    void A_RHITemplateRuntime::FinalizeActivationAndUnlock()
    {
        FinalizeActivation();
        _IsActivated.test_and_set(boost::memory_order_release);
    }
}
