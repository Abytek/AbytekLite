#include "Abytek/RHIBindGroupTemplateRuntimeProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHIBindGroupTemplateRuntime.hpp"
#include "Abytek/DirectX12/RHIBindGroupTemplateRuntimeProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIBindGroupTemplateRuntimeProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIBindGroupTemplateRuntimeProxy>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()

    void A_RHIBindGroupTemplateRuntimeProxy::LateBuildBindGroupTemplateRuntime()
    {
        auto BindGroupTemplateRuntime = GetBindGroupTemplateRuntime();
    }
    void A_RHIBindGroupTemplateRuntimeProxy::Release()
    {
        A_RHITemplateRuntimeProxy::Release();
    }
}
