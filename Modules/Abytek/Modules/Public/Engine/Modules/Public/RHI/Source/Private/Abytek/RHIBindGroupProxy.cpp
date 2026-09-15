#include "Abytek/RHIBindGroupProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIBindGroupTemplateRuntime.hpp"
#include "Abytek/RHIBindGroupTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12/RHIBindGroupProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIBindGroupProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIBindGroupProxy>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHIBindGroupProxy::LateBuildBindGroup()
    {
        auto BindGroup = GetBindGroup();
        _TemplateRuntimeProxy = BindGroup->GetTemplateRuntime()->GetProxy().FastCast<A_RHIBindGroupTemplateRuntimeProxy>();
    }
    void A_RHIBindGroupProxy::Release()
    {
        _TemplateRuntimeProxy = {};
        A_RHIContextChildProxy::Release();
    }
}
