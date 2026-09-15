#include "Abytek/DirectX12/RHICommandQueue.hpp"

#include "Abytek/RHIContext.hpp"
#include "Abytek/DirectX12/RHICommandQueueProxy.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIProcess.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHICommandQueue);
    void F_DirectX12RHICommandQueue::Build(const F_DirectX12RHICommandQueueBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        
        _CommandListType = BuildParams.CommandListType;
        _Flags = BuildParams.Flags;
        
        GetProxy().FastCast<F_DirectX12RHICommandQueueProxy>()->LateBuildCommandQueue_DirectX12();
        
        DirectX12RHIProcessQueries::Execution::F_InitCommandQueue Query;
        Query.CommandQueueProxy = GetProxy().FastCast<F_DirectX12RHICommandQueueProxy>();
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()->Queues.Execution.InitCommandQueue.Push(Query);
    }
    void F_DirectX12RHICommandQueue::Release()
    {
        _Flags = DirectX12SharedAPIWrapper::E_CommandQueueFlag::NONE;
        _CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::NONE;
        A_RHIContextChild::Release();
    }

    TS<A_RHIContextChildProxy> F_DirectX12RHICommandQueue::CreateProxy()
    {
        return RACreateAndBuildShared<F_DirectX12RHICommandQueueProxy>(ABYTEK_WTHIS());
    }
}
#endif