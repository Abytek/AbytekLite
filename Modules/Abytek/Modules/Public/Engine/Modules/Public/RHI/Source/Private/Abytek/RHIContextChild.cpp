#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIContextProxy.hpp"


namespace Abytek
{
    void A_RHIContextChildProxy::Build(const TW_Valid<A_RHIContextChild>& ContextChild)
    {
        A_RAObject::BuildMinimal();
        _ContextProxy = ContextChild->GetContext()->GetProxy().Weak();
        _ContextChild = ContextChild;
    }
    void A_RHIContextChildProxy::Release()
    {
        _ContextChild = {};
        _ContextProxy = {};
        A_RAObject::Release();
    }

    void A_RHIContextChild::Build(const F_RHIContextChildBuildParams& BuildParams)
    {
        A_RAObject::BuildMinimal();
        _Context = BuildParams.Context;
        
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        
        _Proxy = CreateProxy();
    }

    void A_RHIContextChild::Release()
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        
        DestroyProxy();
        _Context = {};
        A_RAObject::Release();
    }

    TS<A_RHIContextChildProxy> A_RHIContextChild::CreateProxy()
    {
        return {};
    }
    void A_RHIContextChild::DestroyProxy()
    {
        _Proxy = {};
    }

#ifdef ABYTEK_DEBUG_INFO
    void A_RHIContextChild::SetDebugName(const F_DebugName& DebugName) noexcept
    {
        A_RAObject::SetDebugName(DebugName);
        if (_Proxy)
        {
            _Proxy->SetDebugName(F_Name(ToText(*DebugName) + ABYTEK_TEXT(".Proxy")));
        }
    }
#endif

    B8 A_RHIContextChild::UseStrictMode() const
    {
        return true;
    }
}