#include "Abytek/RHISubsystemProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_RHISubsystemProxy);
    ABYTEK_RA_BEGIN_OBJECT(A_RHISubsystemProxy)
        const auto& APINames = F_RHISubsystem::GetInstance()->GetConsoleVariable_APINames()->GetValue();
        for (auto APIName : APINames)
        {
            E_RHIAPI API = E_RHIAPI::NONE;
            if (
                auto Status = RHIParseAPI(APIName, API);
                !Status
            )
            {
                ABYTEK_LOG_FATAL() << Status.ErrorMessage;
            }
            
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
            if (API == E_RHIAPI::DIRECTX12)
            {
                if (auto SubsystemProxy = F_DirectX12RHISubsystemProxy::TryCreate())
                {
                    return SubsystemProxy;
                }
            }
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHISubsystemProxy::Build()
    {
        A_RAObject::BuildMinimal();

        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    void A_RHISubsystemProxy::Release()
    {
        A_RAObject::Release();
    }

    void A_RHISubsystemProxy::OnBeginFrameParam(E_FrameParamType FrameParamType)
    {
    }
    void A_RHISubsystemProxy::OnEndFrameParam(E_FrameParamType FrameParamType)
    {
    }
}
