#include "Abytek/RenderPackProxy.hpp"
#include "Abytek/RenderCoreHelper.hpp"


namespace Abytek
{
    F_RenderPackProxy::F_RenderPackProxy(const TW_Valid<A_Renderable>& Owner) :
        A_RenderProxy(Owner)
    {
    }
    F_RenderPackProxy::~F_RenderPackProxy()
    {
    }

    void F_RenderPackProxy::OnInit_RenderTask()
    {
    }
    void F_RenderPackProxy::OnRelease_RenderTask()
    {
    }

    void F_RenderPackProxy::OnCreateRenderState_RenderTask()
    {
    }
    void F_RenderPackProxy::OnDestroyRenderState_RenderTask()
    {
        _TemplateRuntimes = {};
        _RegistryRuntime = {};
        _Registry = {};
    }

    TS<A_RHITemplateRuntime> F_RenderPackProxy::GetTemplateRuntime(F_RHITemplateHashCode HashCode)
    {
        auto Template = GetTemplate(HashCode);
        return _RegistryRuntime->GetTemplateRuntimeDatabase()->GetOrActivateRuntime(Template);
    }

    void F_RenderPackProxy::OnAddTemplate(const TS<A_RHITemplate>& Template)
    {
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(_RegistryRuntime);
        auto TemplateRuntimeDatabase = _RegistryRuntime->GetTemplateRuntimeDatabase();
        _TemplateRuntimes.push_back(
            TemplateRuntimeDatabase->GetOrActivateRuntime(Template)
        );
    }
}
