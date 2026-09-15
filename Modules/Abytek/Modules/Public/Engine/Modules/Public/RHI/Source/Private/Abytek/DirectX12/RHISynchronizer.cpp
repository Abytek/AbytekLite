#include "Abytek/DirectX12/RHISynchronizer.hpp"
#include "Abytek/DirectX12/RHIFence.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHISynchronizer);
    void F_DirectX12RHISynchronizer::Build(const F_DirectX12RHISynchronizerBuildParams& BuildParams)
    {
        A_RHIDeviceChild::Build(BuildParams.Device);
        F_DirectX12RHIFenceBuildParams FenceBuildParams;
        FenceBuildParams.Device = BuildParams.Device;
        FenceBuildParams.InitialValue = 0;
        _Fence = RACreateAndBuild<F_DirectX12RHIFence>(FenceBuildParams);
        _LastSignaledValue = 0;
        _NextAllocatedValue = 1;
    }
    void F_DirectX12RHISynchronizer::Release()
    {
        _LastSignaledValue = 0;
        _NextAllocatedValue = 1;
        _Fence = {};
        A_RHIDeviceChild::Release();
    }

    U64 F_DirectX12RHISynchronizer::AllocateNewValue()
    {
        return _NextAllocatedValue.fetch_add(1, boost::memory_order_release);
    }
    void F_DirectX12RHISynchronizer::SignalFromGPUSide(U64 Value, const TW_Valid<F_DirectX12RHICommandQueueProxy>& CommandQueueProxy)
    {
        while (true)
        {
            B8 Done = false;
            _SignalCriticalSection(
                [this, Value, CommandQueueProxy, &Done]
                {
                    if (_LastSignaledValue < (Value - 1))
                    {
                        return;
                    }
                    _Fence->SignalFromGPUSide(Value, CommandQueueProxy);
                    _LastSignaledValue = Value;
                    Done = true;
                }
            );
            if (Done)
            {
                break;
            }
            H_TaskUtilities::Switch();
        }
    }
    void F_DirectX12RHISynchronizer::WaitOnCPUSide(U64 Value)
    {
        _Fence->WaitOnCPUSide(Value);
    }
    void F_DirectX12RHISynchronizer::WaitOnGPUSide(U64 Value, const TW_Valid<F_DirectX12RHICommandQueueProxy>& CommandQueueProxy)
    {
        _Fence->WaitOnGPUSide(Value, CommandQueueProxy);
    }
}
#endif